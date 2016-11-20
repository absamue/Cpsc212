
#include <iostream>
#include <cstdlib>
using namespace std;

struct Node {
	int key;
	int size;
	Node *left;
	Node *right;
	Node (int k) { key = k; size = 1; left = right = NULL; }
};

void fix_size(Node *T)
{
	T->size = 1;
	if (T->left) T->size += T->left->size;
	if (T->right) T->size += T->right->size;
}



void print_inorder(Node *T)
{
	if (T == NULL) return;
	print_inorder(T->left);
	cout << T->key << "\n";
	print_inorder(T->right);
}


void split(Node *T, int r, Node **L, Node **R)
{
	if(T == NULL){
		*L = NULL;
		*R = NULL;
		return;
	}

	int rank_of_root = T->left ? T->left->size : 0;

	if(r <= rank_of_root){
		split(T->left, r, L, &T->left);
		*R = T;
	}
	else {
		split(T->right, r - rank_of_root-1, &T->right, R);
		*L = T;
	}
	fix_size(T);

}


Node *insert_random(Node *T, int v, int r){
	// If k is the Nth node inserted into T, then:
	// with probability 1/N, insert k at the root of T
	// otherwise, insert_random k recursively left or right of the root of
	// T
	if (T == NULL) return new Node(v);
	if (rand() % (T->size + 1) == 0) {
		// insert at root
		Node *new_root = new Node(v);
		split(T, r, &new_root->left, &new_root->right);
		fix_size(new_root);
		return new_root;
	}
	// otherwise insert recursively...
	// TBD: you'll need to fill in this part

	int rank_of_root = T->left ? T->left->size : 0;
	if(r <= rank_of_root) 
		T->left = insert_random(T->left, v, r);
	else 
		T->right = insert_random(T->right, v, r-rank_of_root-1);
	fix_size(T);
	return T;
}

int get_rand(int N){
	return 123456789 % N;
}

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

	//generate bad input for prog3. Prog3's quicksort uses a random pivot which
	//will give predictable numbers, and if printing the minimum into the
	//pivot the sort will run in n^2 time as each pivot will split the array
	//into a 1 element and N-1 element lists
	cout << N << "\n";
	Node *R = NULL;
	for(int i=1; i<=N; i++){
		R = insert_random(R, i, get_rand(i));
	}

	print_inorder(R);
	return 0;
}
