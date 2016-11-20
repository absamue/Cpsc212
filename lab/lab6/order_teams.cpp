#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include <set>

using namespace std;

struct Node {
  int val;
  Node *next;
  Node (int v, Node *n) { val = v; next = n; }
};

// Returns true if team x defeated team y
bool did_x_beat_y(int x, int y)
{
  if (x > y) return !did_x_beat_y(y,x);
  unsigned long long lx = x;
  unsigned long long ly = y;
  return ((17 + 8321813 * lx + 1861 * ly) % 982451653) % 2 == 0;
}

// TBD: Please change this to work in a non-recursive fashion.
// The current recursive implementation works but it runs out of 
// stack space for large problem sizes... (a pity, since it's so 
// simple to write this function recursively...)

Node *merge(Node *left, Node *right){
	
	//check if one side is NULL
	Node *merge;
	if(left == NULL){
		merge = right;
		return merge;
	}
	if(right == NULL){
		merge = left;
		return merge;
	}

	//determine the first element
	if(did_x_beat_y(left->val, right->val)){
		merge = left;
		left = left->next;
	}
	else{ 
		merge = right;
		right = right->next;
	}
			
	//keep track of the head
	Node *head = merge;	

	//compare each element and insert the winner next
	while(left != NULL && right != NULL){
		if(did_x_beat_y(left->val, right->val)){
			merge->next = left;
			left = left->next;
			merge = merge->next;
		}
		else{
			merge->next = right;
			right = right->next;
			merge = merge->next;
		}
	}

	//if lists are different lengths, add the leftovers
	if(left==NULL)
		merge->next = right;
	if(right==NULL)
		merge->next = left;

	return head;

	//delete old lists
	Node *temp;
	while(left){
		temp = left->next;
		delete left;
		left = temp;
	}
	while(right){
		temp = right->next;
		delete right;
		right = temp;
	}
}

// Return a linked list containing a valid ordering of teams start..end
Node *list_ordering(int start, int end)
{
  if (start == end) {
    return new Node(start, NULL);
  }
  int mid = (start+end)/2;
  Node *left = list_ordering(start,mid);
  Node *right = list_ordering(mid+1,end);
  return merge(left, right);
}

// Return an array containing a valid ordering of teams start..end
int *array_ordering(int start, int end)
{
  // TBD... the following line is here just to prevent a seg fault when
  // running initially; it returns an array of the correct size but filled
  // with garbage values, so it won't pass the checks in main()...

  //check if comparing the same thing
	if(start == end){
	  int *A = new int[1];
	  A[0] = start;
	  return A;
  }
	
	//recursively set up two sorted ararys and an array to hold the merge
	int mid = (start + end)/2;
	int *L = array_ordering(start, mid);
	int *R = array_ordering(mid+1, end);
	int *merge = new int[end-start+1];

	//place the winner of each list in correct order
	int i=0, j=0, k=0;
	while(i<=mid-start && j<=end-mid-1){
		if(did_x_beat_y(L[i], R[j])){
			merge[k++] = L[i++];
		}
		else{
			merge[k++] = R[j++];
		}
	}

	//copy the remaining teams if the two arrays are uneven
	if(i<=mid-start){
		while(i<=mid-start){
			merge[k++] = L[i++];
		}
	}
	if(j<=end-mid-1){
		while(j<=end-mid-1){
			merge[k++] = R[j++];
		}
	}

	//delete placeholder ararys and return sorted array
	delete [] L;
	delete [] R;
	return merge;
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "You need to specify the number of teams on the command line\n";
    return 0;
  }

  int Nteams = atoi(argv[1]);
  cout << "Testing with " << Nteams << " teams.\n";

  cout << "Checking list-based implementation...\n";
  Node *L = list_ordering(0,Nteams-1);
  bool correct = true;
  set<int> teams_used;
  for (int i=0; i<Nteams-1; i++) {
    if (L == NULL || L->next == NULL) {
      cout << "List appears to be too short!\n";
      correct = false; break;
    } else if (L->val==L->next->val || teams_used.count(L->val)>0) {
      cout << "Team " << L->val << " appears twice in your ordering!\n";
      correct = false; break;
    } else if (L->val<0 || L->val>=Nteams) {
      cout << "Invalid team " << L->val << " appears in your ordering!\n";
      correct = false; break;
    } else if (!did_x_beat_y(L->val, L->next->val)) {
      cout << "Invalid order: team " << L->val << " (position " << i <<
	") lost to team " << L->next->val << " (position " << i+1 << ")\n";
      correct = false; break;
    }
    teams_used.insert(L->val);
    Node *to_delete = L;
    L = L->next;
    delete to_delete; // free elements of list as we go...
  }
  if (L != NULL && L->next != NULL) {
    cout << "List appears too long!\n";
    correct = false;
  }
  if (correct) cout << "Correct answer!\n";
  if (L) delete L; // delete last element

  cout << "Checking array-based implementation...\n";  
  int *A = array_ordering(0,Nteams-1);
  correct = true;
  teams_used.clear();
  for (int i=0; i<Nteams-1; i++) {
    if (A[i]==A[i+1] || teams_used.count(A[i])>0) {
      cout << "Team " << A[i] << " appears twice in your ordering!\n";
      correct = false; break;
    } else if (A[i]<0 || A[i]>=Nteams) {
      cout << "Invalid team " << A[i] << " appears in your ordering!\n";
      correct = false; break;
    } else if (!did_x_beat_y(A[i], A[i+1])) {
      cout << "Invalid order: team " << A[i] << " (position " << i <<
	") lost to team " << A[i+1] << " (position " << i+1 << ")\n";
      correct = false; break;
    }
    teams_used.insert(A[i]);
  }
  if (correct) cout << "Correct answer!\n";
  delete [] A; // free memory used by array
  
  return 0;
}
