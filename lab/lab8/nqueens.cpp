#include <stdio.h>
#include <iostream>

using namespace std;

int N;

bool check(int **board, int row, int i, int col){

	//check vertically for a queen
	int c = row - 1;
	while(c >= 0)
	{
		if(board[c][i]==1)
			return true;
		c--;
	}

	//check the diagonals
	c=row-1;
	int d=i+1, e=i-1;
	while(c >=0){
		if(d<col){
			if(board[c][d]==1)
				return true;
			d++;
		}
		
		if(e>=0){
			if(board[c][e] == 1)
				return true;
			e--;
		}
		c--;
	}

	return false;
}

int solve(int **board, int row, int col){

	int total = 0;
	//base case
	if(row == col)
		return 1;

	for(int i=0; i < col; i++){
		if(check(board, row, i, col) == false){
			board[row][i] = 1;

			total += solve(board, row+1, col);

			board[row][i] = 0;
		}
	}
	
	return total;
}

int main(void){
	
	int n;
	cout << "Please enter a value for N: \n";
	cin >> n;
	
	//start with empty array
	int **board = new int*[n];
	for(int i=0; i<n; i++){
		board[i] = new int[n];
	}
	
	//set board to be empty
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			board[i][j] = 0;
		}
	}

	int total = solve(board, 0, n);
		cout << "total: " << total << "\n";
	return 0;
}
