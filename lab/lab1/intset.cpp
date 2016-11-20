#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{ 
  first = NULL;
}

Intset::~Intset()
{
  delete[] A;
}

/* Return true if key is in the set */
bool Intset::find(int key)
{
	Node *current = first;

	while(current)
	{
		if(current->val == key)
			return true;
		
		current = current->next;
	}

	return false;
}


/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
	//check for the key
	assert(!find(key));

	//add to beginnig of empty list
	if(first == NULL)
	{
		first = new Node;
		first->val = key;
		first->next = NULL;
		return;
	}
	
	//if list is not empty traverse and add to end
	else
	{
		Node *current = first;

		while(current->next != NULL && current->next->val < key){
				current = current->next;	
		}
		
		//add new link either inbetween or at end
		Node *link = new Node;
		if(current->next != NULL)
			link->next = current->next;
		else
			link->next = NULL;

		current->next = link;
		link->val = key;
	}

}



/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{

	assert (find(key));
	
	Node *current = first;
	if(current->next == NULL){
		delete first;
		return;
	}

	if(current->val == key){
		first = current->next;
		delete current;
		return;
	}

	while(current->next->val != key){
		current = current->next;
	}

	Node *temp = current->next;
	current->next = current->next->next;
	delete temp;

}

void Intset::print(void)
{
	Node *current = first;
	while(current->next != NULL){
		cout << current->val << "\n";
		current = current->next;
	}

	cout << current->val << "\n";
}
