#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;

int N, D, K;

struct Node{
	double score;
	double guess;
	double dist;
	double dim[11];
	Node *left;
	Node *right;
	Node(double s){score = s;}
	Node(){score = -1; left = right = NULL;}
};

struct CompareDist{
	bool operator()(Node const & A, Node const & B){
		return A.dist < B.dist;
	}
};

std::priority_queue<Node, vector<Node>, CompareDist> Q;


//global KD tree holder
Node *T = NULL;

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
double calc_dist(Node *A, Node *B){
	double d = 0;
	for(int i=0; i<D; i++){
		d += pow((B->dim[i] - A->dim[i]), 2);
	}

	return sqrt(d);
}

bool comp(double *T, double *search){
	for(int i=0; i<D; i++)
		if(T[i] != search[i]) return false;

	return true;
}

void search(Node *T, Node *in, int d){	
	if(in->score == T->score && comp(T->dim, in->dim)) return;
	
	T->dist = calc_dist(T, in);
	Q.push(*T);
	if(Q.size() > K) Q.pop();

	if(in->dim[d] < T->dim[d])
		search(T->left, in, (d+1)%D);
	else
		search(T->right, in, (d+1)%D);
	
}

void print_inorder(Node *T){
	if(T==NULL) return;
	print_inorder(T->left);
	cout << T->score << "\n";
	print_inorder(T->right);
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

	for(int i=0; i<N; i++){
		T = insert(T, table[i], 0);
	}

	for(int i=0; i<N; i++){
		search(T, table[i], 0);
	}

	return 0;
}
