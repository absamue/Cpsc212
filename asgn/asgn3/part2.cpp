/*
Andrew Samuels
CPSC 212 - Algorithms and Data Structures

Assignment 3, Iterative Optimization

Given a graph contianing number-identified nodes and their connections, use
iterative refinement to place the nodes in an ordered line so that each
node is as close to its connections as possible

This method relaxes the constraint that each node must be given an integer
position during the refinement process
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <climits>
using namespace std;

int N = 936;

struct Node{
	int val;
	double pos;
	Node *next;
	Node() {val = -1; next = NULL;}
	Node(int v, Node *n){val = v; next = n;}
};

int find(Node **table, int search){
	for(int i=0; i<N; i++){;
		if(table[i]->val == search)
			return i;
	}

	cout << "not found\n";
	return -1;
}


//read in text points
Node **read(void){
	ifstream wordfile("wires.txt");

	int x, y;
	//initialize table
	Node **table = new Node *[N];
	for(int i=0; i<N; i++) 
		table[i] = NULL;

	Node *temp, *check;
	//read in info
	while(wordfile >> x >> y){
		temp = table[x]; 
		check = table[y];

		//No node at this position yet
		if(temp == NULL){ 
			table[x] = new Node(x, NULL);
			table[x]->next = new Node(y, NULL);
		}
		//Node present, add its link to list
		else{
			while(temp->next != NULL) temp = temp->next;
			temp->next = new Node(y, NULL);
		}

		//check if link is present, and if not then create it
		if(!check){
			table[y] = new Node(y, NULL);
			table[y]->next = new Node(x, NULL);
		}
		else{
			while(check->next != NULL) check = check->next;
			check->next = new Node(x, NULL);
		}
	}

	return table;
}

//print out the current table
void print(Node **table){
	for(int i=0; i<N; i++){
		Node *temp = table[i];
		while(temp){
			cout << temp->val << " ";
			temp = temp->next;
		}
		cout << "\n";
	}

}

//swap random elements
Node **randomize(Node **table){
	srand(time(0));
	for(int i=0; i<N; i++){
		swap(table[i], table[rand() % N]);
	}
	return table;
}


double find_pos(Node **table, int search){
	for(int i=0; i<N; i++){
		if(table[i]->val == search)
			return table[i]->pos;
	}

	cout << "position not found\n";
	return -1;
}

double average(Node **table, Node *cur){
	double total = 0;
	int count = 0;

	Node *temp = cur->next;
	while(temp){
		count++;
		total += find_pos(table, temp->val);
		temp = temp->next;
	}

	double pos = (total/count);
	cur->pos = pos;

	return pos;
}

Node **iterate(Node **table){
	double delta = 0;
	double last = 1;
	double pre, post;
	double test = 1000000;

	for(int i=1; i<N-1; i++){
		table[i]->pos = (rand() % N);
	}
	table[0]->pos = 0;
	table[N-1]->pos = N-1;

	while(test > 0.01){
		for(int i=1; i<N-1; i++){
			pre = table[i]->pos;
			table[i]->pos = average(table, table[i]);
			post = table[i]->pos;
			delta += fabs(post - pre);
		}
		test = fabs(delta - last);
		//		cout << "test: " << test << "\n";
		//		cout << "delta: " << delta << "\n";
		//		cout << "pre: " << pre << "\n";
		last = delta;
		delta = 0;
	}
	return table;
}


//calculate distance of entire path
int calc_dist(Node **table){
	int count = 0;
	Node *temp;
	for(int i=0; i<N; i++){
		temp = table[i]->next;
		while(temp != NULL){
			int dis = find(table, temp->val);
			count += pow(abs(i - dis), 2);
			temp = temp->next;
		}
	}
	return count;
}

//bubble sort
Node **sort(Node **table){
	for(int i=1; i<N-1; i++){
		for(int j=0; j<N-i-1; j++){
			if(table[j]->pos > table[j+1]->pos)
				swap(table[j], table[j+1]);
		}
	}

	return table;
}

int main(void){
	Node **table, **opt, **rand, **lowest;
	int min, test;
	
	table = read();
	cout << "Read input.\n\n";
	min = calc_dist(table); //starting point

	cout << "Working." << flush; //loading bar of sorts
	//make sure we got a minimum of at least under 1mil
	while(min > 1000000){
		for(int i=0; i<5; i++){
			rand = randomize(table);
			opt = iterate(rand);
			opt = sort(opt);

			test = calc_dist(opt);
			if(test < min){
				min = test;
				lowest = opt;
			}
			cout << "." << flush;
		}
		//print out total distance (divided by two since it was stored as
		//bi-directional connections)
		min = (calc_dist(lowest)) / 2;
		cout << min << "\n"; //current attempt
	}
	print(lowest);
	cout << "\n" << min << "\n";


	return 0;
}
