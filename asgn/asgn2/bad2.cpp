
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cout << "Usage: bad2 <input size>\n";
		return 0;
	}

	int N = atoi(argv[1]);  // get first command-line argument
	if (N<1 || N>100000) {
		cout << "Invalid input size!\n";
		return 0;
	}

	// Generate and print bad input of size N for prog2
	//since the hash function computes by modulus divison of 10 thousand,
	//inserting increments of 10 thousand will all hash to one list
	cout << N << "\n";
	int count = 0;
	for (int i=0; i<N; i++){
		cout << count << "\n";
		count += 10000;
	}
		return 0;
}
