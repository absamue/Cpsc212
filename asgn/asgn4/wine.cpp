/*
Andrew Samuels
CPSC 212 - Algorithms and Data strutures

Assignment 4, K Nearest Neighbor Search

Given data of wine bottles that contains each bottle's score and ratings of
different attributes of the wine, use a KD tree and a KNN search to attempt
guessing the value of each bottle's actual score
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define INF 100000
int N, D, K;
double sum = 0;

struct Node{
	double score;
	float weight;
	double dim[11];
	bool reached;
	Node *left;
	Node *right;
	Node(double s){score = s; reached = false;}
	Node(){score = -1; left = right = NULL; reached = false;}
};


//global KD tree holder
Node *T = NULL;

//psuedo queue because I couldn't figure out how to use an actual queue
typedef pair<double, Node *> Point;
vector<Point> V;

void normalize(Node **table){
	double count, avg, var, dev;
	int i;
	//perform process on each dimension
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

	//normalize the data so that all the points are standardized
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

//KD tree insertion
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

//checks if two nodes are identical, used for recursing to the desired node
//during the search process
bool comp(Node *T, Node *in){
	//not same score, dim check unnessecarry
	if(T->score != in->score) return false;
	//check if dims are the same
	for(int i=0; i<D; i++)
		if(T->dim[i] != in->dim[i]) return false;

	//found a match
	return true;
}


//make sure a Node isn't already in the neighbor list
bool check(Node *T){
	for(int i=0; i<K; i++){
		if(V[i].second != NULL)
			if(comp(T, V[i].second)) return false;
	}
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
		//node wasn't found, continue searching
		if(in->reached == false){
			if(in->dim[d] < T->dim[d])
				search(T->left, in, (d+1)%D);
			else
				search(T->right, in, (d+1)%D);
		}
	}
	
	//insert the current Node, and keep only K nodes
	//sorts K neighbors by distance to easily remove the furthest neighbor
	if(!comp(T, in) && check(T))
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
	
	//calculate the average score of in's nearest points
	for(int i=0; i<K; i++){
		if(V[i].second == NULL) continue;
		V[i].second->weight = pow(e, -.01 * V[i].first);
		num += V[i].second->weight * V[i].second->score;
		denom += V[i].second->weight;
	}


	in->weight = (float)(num / denom);

	//keep a running total of the average difference
	sum += pow((in->score - in->weight), 2);
	
}

void print(Node **table){
	sum = sum / N;
	cout << "Result: " << sum << "\n";

}

int main(int argc, char *argv[]){
	//number of neighbors to search for
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

	//set up the KD tree
	for(int i=0; i<N; i++){
		T = insert(T, table[i], 0);
	}

	//for each of the points, run a KNN search and guess what it's value is
	for(int i=0; i<N; i++){
		//set up vector to have infinite distances to make it to search
		for(int j=0; j<K; j++) V.push_back(Point(INF, NULL));

		search(T, table[i], 0);
		weigh(table[i]);
		
		//reset the K neighbors
		V.clear();
	}

	print(table);

	return 0;
}
