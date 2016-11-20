#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

struct Node{
	char letter;
	int parent;
	Node(char l, int p){
		letter = l; parent = p;
	}
};

int main(void){
	vector<Node> nodes;
	nodes.push_back(Node(0,-1));

	//build vector of characters and their parents
	int i, c;
	cin >> i >> c;
	while(c != -1){
		nodes.push_back(Node(c, i));
		cin >> i >> c;
	}
	
	vector<int> Q; //FILO "queue to print out in correct order
	for(int i=1; i<nodes.size(); i++){
		//check if the current node has parent
		if(nodes[i].parent != 0){
			//as long as there are parents found, insert their respective
			//letters into the queue
			int test = nodes[i].parent;
			while(test != 0){
				Q.insert(Q.begin(), test);
				test = nodes[test].parent; //get next parent
			}
			
			//print out queued letters in correct order
			for(int i=0; i<Q.size(); i++){
				int pos = Q[i];
				cout << nodes[pos].letter;
			}
			Q.clear();
		}
		//print out current letter after checking parents
		cout << nodes[i].letter;
	}
	return 0;
}
