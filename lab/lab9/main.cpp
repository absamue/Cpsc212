#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

struct state{
	int a;
	int b;
	string n;
	state(int x, int y, string m){a = x; b = y; n = m;}
};

state *prev_state[1001][1001];
bool visited[1001][1001];

int A, B, X;
int end1 = 2000;
int end2 = 2000;

void search(int jug1, int jug2){
	if(visited[jug1][jug2] == true) return;
	
	visited[jug1][jug2] = true;
	
	if((jug1 + jug2) == X){ //found goal
		end1 = jug1;
		end2 = jug2;
		return;
	}

	//empty 1
	if(visited[0][jug2] == false){
		prev_state[0][jug2] = new state(jug1, jug2, "Empty Jug 1");
		search(0, jug2);
	}
	
	//empty 2
	if(visited[jug1][0] == false){
		prev_state[jug1][0] = new state(jug1, jug2, "Empty Jug 2");
		search(jug1, 0);
	}
	
	//fill 1
	if(visited[A][jug2] == false){
		prev_state[A][jug2] = new state(jug1, jug2, "Fill Jug 1");
		search(A, jug2);
	}
	
	//fill 2
	if(visited[jug1][B] == false){
		prev_state[jug1][B] = new state(jug1, jug2, "Fill Jug 2");
		search(jug1, B);
	}
	
	//pour 2>1
	if(jug2 > A - jug1){
		if(visited[A][jug2 - (A - jug1)] == false){
			prev_state[A][jug2 - (A - jug1)] = new state(jug1, jug2, "Pour 2 -> 1");
				search(A, jug2 - (A - jug1));
		}
	}
	else{
		if(visited[jug1+jug2][0] == false){
			prev_state[jug1+jug2][0] = new state(jug1, jug2, "Pour 2 -> 1");
			search(jug1+jug2, 0);
		}
	}
	
	//pour 1>2
	if(jug1 > B - jug2){
		if(visited[jug1 - (B-jug2)][B] == false){
			prev_state[jug1 - (B-jug2)][B] = new state(jug1, jug2, "Pour 1 -> 2");
				search(jug1 - (B-jug2), B);
		}
	}
	else{
		if(visited[0][jug1+jug2] == false){
			prev_state[0][jug1+jug2] = new state(jug1, jug2, "Pour 1 -> 2");
			search(0, jug1+jug2);
		}
	}

}

//print recursively from end to beginning 
void print(int a, int b){
	if(!(prev_state[a][b]->a == 0 && prev_state[a][b]->b == 0))
		print(prev_state[a][b]->a, prev_state[a][b]->b);
	
	cout << prev_state[a][b]->n << "\n"; 
	cout << "A = " << a << ", " << "B = " << b << "\n";
}

int main(void){

	cout << "Enter A: ";
	cin >> A;
	cout << "Enter B: ";
	cin >> B;
	cout << "Enter X: ";
	cin >> X;

	if(A > 1000 || B > 1000){
		cout << "\nValues must be less than or equal to 1000\n";
		main();
	}

	//total volume is less than what wants to be found
	if((A+B) < X){
		cout << "\nImpossible\n";
		main();
	}

	for(int i=0; i<1001; i++){
		for(int j=0; j<1001; j++){
			visited[i][j] = false;
		}
	}

	search(0, 0);
	if(end1 != 2000){
		cout << "Found solution\n";
		print(end1, end2);
	}
	else
		cout << "No solution possible\n";

	return 0;
}
