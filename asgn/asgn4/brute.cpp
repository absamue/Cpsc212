#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define INF 100
int N, D, K;
double sum = 0;


struct Node{
	double score;
	double weight;
	double dim[11];
	bool reached;
	Node *left;
	Node *right;
	Node(double s){score = s; reached = false;}
	Node(){score = -1; left = right = NULL; reached = false;}
};


//global KD tree holder
Node *T = NULL;

typedef pair<double, Node *> Point;
vector<Point> V;

void normalize(Node **table){
	double count, avg, var, dev;
	int i;
	for(int j=0; j<D; j++){
		count = 0;
		//calc average of dimension j
		for(i=0; i<N; i++)
			count += table[i]->dim[j];
		avg = count / N;

		//set up each point to have zero mean
		for(i=0; i<N; i++)
			table[i]->dim[j] -= avg;

		//calculate variance and standard deviation
		for(i=0; i<N; i++)
			var += pow((table[i]->dim[j] - avg), 2);
		var = var / N;
		dev = sqrt(var);

		//assign unit variance to dimension
		for(i=0; i<N; i++)
			table[i]->dim[j] = table[i]->dim[j] / dev;

	}
}

Node **read(char *file){
	double S, P;

	ifstream wordfile(file);
	//get number of points and dimensions
	wordfile >> N >> D;

	Node **table = new Node *[N];

	for(int i=0; i<N; i++){
		//read in the score
		wordfile >> S;
		table[i] = new Node(S);
		//fill in the points for each dimension
		for(int j=0; j<D; j++){
			wordfile >> P;
			table[i]->dim[j] = P;
		}
	}

	wordfile.close();


	normalize(table);

	//randomize the table to create a psuedo-random insertion into the kd
	//tree
	srand(time(0));
	for(int i=0; i<N; i++){
		swap(table[i], table[rand() % N]);
	}


	cout << "Read completed\n";

	return table;
}

Node *insert(Node *T, Node *in, int d){
	//made it to an empty spot, insert here
	if(T == NULL){
		T = in;
		return T;
	}
	//insert like BST, cycling through each dimension
	if(in->dim[d] < T->dim[d]) 
		T->left = insert(T->left, in, (d+1)%D);
	else 
		T->right = insert(T->right, in, (d+1)%D);
	return T;
}


//Euclidean distance in D dimensions
double dist(Node *A, Node *B){
	
	if(A==NULL || B == NULL) return INF;

	double d = 0;
	for(int i=0; i<D; i++){
		d += pow((B->dim[i] - A->dim[i]), 2);
	}

	return sqrt(d);
}

bool comp(Node *T, Node *in){
	//not same score, dim check unnessecarry
	if(T->score != in->score) return false;
	//check if dims are the same
	for(int i=0; i<D; i++)
		if(T->dim[i] != in->dim[i]) return false;

	//found a match
	return true;
}

void search(Node *T, Node *in, int d){	
	
	if(T == NULL) return;

	//recurse down to the node we are searching for
	if(in->reached == false){
		//we made it there, mark it as reached
		if(comp(T, in)){
			in->reached = true;
		}
		if(in->reached == false){
			if(in->dim[d] < T->dim[d])
				search(T->left, in, (d+1)%D);
			else
				search(T->right, in, (d+1)%D);
		}
	}
	
	//insert the current Node, and keep only K nodes
	if(!comp(T, in))
		V.push_back(Point(dist(T, in), T));
	sort(V.begin(), V.end());
	if(V.size() > K)
		while(V.size() > K) V.pop_back();

	//if the distance to the left is less than that of V.end, search the
	//left tree 
	double test = dist(T->left, in);
	if(test < V.back().first)
		search(T->left, in, (d+1)%D);
	//check distance of right tree to end, and if less check right tree
	else{
		test = dist(T->right, in);
		if(test < V.back().first)
			search(T->right, in, (d+1)%D);
	}
}


void weigh(Node *in){
	double num = 0;
	double denom = 0;
	double e = 2.71828;
	for(int i=0; i<K; i++){
		if(V[i].second == NULL) continue;
		V[i].second->weight = pow(e, -0.1 * V[i].first);
		num += V[i].second->weight * V[i].second->score;
		denom += V[i].second->weight;
	}

	in->weight = num / denom;

	sum += pow((in->score - in->weight), 2);
	
}

void print(Node **table){	
	sum = sum / N;
	
	cout << "Result: " << sum << "\n";
	
}

void brute(Node **table){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(i==j) continue;
			V.push_back(Point(dist(table[i], table[j]), table[j]));
			sort(V.begin(), V.end());
			if(V.size() > K)
				while(V.size() > K) V.pop_back();
		}
		weigh(table[i]);
		V.clear();
	}
}

int main(int argc, char *argv[]){
	K = atoi(argv[2]);

	if(argc < 3){
		cout << "Please run the program in the following format:\n";
		cout << "./a.out <input.txt> <K>\n";
		return 0;
	}
	if(K > 10 || K < 1){
		cout << "Please enter a value of K in the range 1-10\n";
		cout << "Format: ./a.out <input.txt> <K>\n";
		return 0;
	}

	Node **table = read(argv[1]);

	brute(table);	

	print(table);

	return 0;
}
