/*
Andrew Samuels
CPSC 212 - Algorithms and Data Structures

Assignment 3, Iterative Optimization

Given a graph contianing number-identified nodes and their connections, use
iterative refinement to place the nodes in an ordered line so that each
node is as close to its connections as possible
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
	int val, dis;
	Node *next;
	Node() {val = -1; next = NULL;}
	Node(int v, Node *n){val = v; next = n;}
};

//returns where the node you are looking for is in the array
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
		if(!temp){ 
			table[x] = new Node(x, NULL);
			table[x]->next = new Node(y, NULL);
		}
		//Node present, add its link to list
		else{
			while(temp->next) temp = temp->next;
			temp->next = new Node(y, NULL);
		}

		//check if link is present, and if not then create it
		if(!check){
			table[y] = new Node(y, NULL);
			table[y]->next = new Node(x, NULL);
		}
		else{
			while(check->next) check=check->next;
			check->next = new Node(x, NULL);
		}
	}

	cout << "Input complete.\n";	
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

//calculate total squared distance of one node's connections
int calc_edge(Node **table, Node *search){
	int count = 0;
	int dis;
	int pos = find(table, search->val);
	Node *temp = search->next;
	//node with edges
	while(temp){
		dis = find(table, temp->val);
		count += pow(abs(pos - dis), 2);
		temp = temp->next;
	}
	return count;
}

//swap nodes until most of them are not swapped to better positions
Node **iterate(Node **table){
	int pre1, pre2, post1, post2;
	int count = 0;
	int swapped = 1;
	Node *check;
	//keep going until most of the nodes are "satisfied", this cant be too
	//large as some nodes will want to place themselves in the same position
	//as other nodes, meaning that they will keep swapping with each other
	while(swapped <350){
		swapped = 0;//start at nothing swapped
		for(int i=0; i<N-1; i++){
			//get the edge distances of the nodes about to be swapped
			pre1 = calc_edge(table, table[i]);
			pre2 = calc_edge(table, table[i+1]);

			swap(table[i], table[i+1]);

			//see if BOTH nodes are better off. Only checking one will result
			//in the loop running endlessly as it optimizes one and makes the
			//other worse off
			post1 = calc_edge(table, table[i+1]);
			post2 = calc_edge(table, table[i]);
			if((pre1 < post1) && (pre2 < post2)){
				swap(table[i], table[i+1]);
				swapped++;
			}

		}
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

int main(void){
	Node **table, **opt, **rand, **lowest;
	int min, test;
	table = read();
	cout << "Read input.\n";
	min = calc_dist(table); //starting distance to compare to

	cout << "Working." << flush; //loading bar so you know its working
	//make sure we get a result under 1mil
	while(min > 1000000){
		for(int i=0; i<3; i++){
			rand = randomize(table);
			opt = iterate(rand);
			test = calc_dist(opt);
			if(test < min){
				min = test;
				lowest = opt;
			}
			cout << "." << flush;
		}
		min = (calc_dist(lowest)) / 2;
		cout << "\n" << min << "\n";
	}
	print(lowest);
	cout << "\n" << min;
	return 0;
}
