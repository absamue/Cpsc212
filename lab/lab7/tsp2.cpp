#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <math.h>
using namespace std;

struct Node{
	double x, y;
	int rank;
	Node () {x = y = -1; rank = -1;}
	Node(double x_in, double y_in, int r){x = x_in; y = y_in; rank = r;}
};

double dist(Node *A, Node *B){
	double dist = sqrt(pow((B->x - A->x), 2) + pow((B->y - A->y), 2));
	return dist;
}


double calc_dist(Node **table){
	double dis = 0;
	for(int i=0; i<50; i++){
		dis += dist(table[i], table[(i+1)%50]);
	}
	return dis;
}

void print(Node **table){
	for(int i=0; i<50; i++)
		cout << table[i]->rank << " ";

	cout << "\n";
	cout << "dist: " << calc_dist(table) << "\n";
}

//read in input and give each point an identifier
Node **read(void){
	Node **table = new Node *[50];
	ifstream wordfile("tsp_points.txt");

	double x, y;
	int i = 0;
	while(wordfile >> x >> y){
		table[i] = new Node(x, y, i);
		i++;
	}
	return table;
}

Node **randomize(Node **table){
	srand(time(0));
	for(int i=0; i<50; i++){
		swap(table[i], table[rand() % 50]);
	}
	return table;
}


Node **iterate(Node **table){
	double low;
	double test;
	for(int i=0; i<48; i++){
		low = dist(table[i], table[i+1]);
		for(int j=i+2; j<50; j++){
			swap(table[i+1], table[j]);
			test = dist(table[i], table[i+1]);
			if(test < low){
				low = test;
			}
			else{
				swap(table[i+1], table[j]);
			}
		}
	}

	return table;
}

void refine(void){
	Node **table = read();

	Node **rand;
	Node **lowest;
	print(table);
	double low = calc_dist(table);
	double test;

	for(int i=0; i<500; i++){
		rand = randomize(table);
		Node **path = iterate(rand);
		test = calc_dist(path);
		if(test < low){
			low = test;
			lowest = path;
		}
	}
	print(lowest);
}


int main(void){
	refine();
	return 0;
}
