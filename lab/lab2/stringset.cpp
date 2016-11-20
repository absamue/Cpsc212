#include <iostream>
#include <string.h>
#include <assert.h>
#include "stringset.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int hash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

/* Allocate a table of pointers to nodes, all initialized to NULL */
Node **allocate_table(int size)
{
  Node **table = new Node *[size];
  for (int i=0; i<size; i++)
    table[i] = NULL;
  return table;
}

Stringset::Stringset()
{
  size = 4; // initial size of table    
  table = allocate_table(size);
  num_elems = 0; 
}

Stringset::~Stringset()
{
  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      Node *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;
}

/* Return true if key is in the set */
bool Stringset::find(string key)
{
  int h = hash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key) return true;
    n = n->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(string key)
{
  assert (!find(key));
  num_elems++;

  if (num_elems == size) {
    // TBD: Expand table -- allocate new table of twice the size,
    // re-insert all keys into new table, and de-allocate old table.
	 size *= 2;
	 Node **old_table = table;
	 table = allocate_table(size);
	 num_elems = 0;
	 
	 for(int i = 0; i < size / 2; i++){
		 Node *temp = old_table[i];
		 while(temp != NULL){
			 insert(temp->key);
			 Node *holder = temp;
			 temp = temp->next;
			 delete holder;
		 }
	 }
	delete [] old_table;

  }

  // TBD: Insert new element
  int h = hash(key, size);
	  table[h] = new Node(key, table[h]);
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(string key)
{
  assert (find(key));
  num_elems--;

  // TBD: Delete element
  int h = hash(key, size);
  Node *current = table[h];
  //list is empty
  if(current->next == NULL){
	  delete table[h];
	  table[h] = NULL;
	  return;
  }
	//key is first in list
  if(current->key == key){
	  table[h] = current->next;
	  delete current;
	  return;
  }

  while(current->key != key){
	  current = current->next;
  }

  Node *temp = current;
  current = current->next;
  delete temp;
}

void Stringset::print(void)
{
  // TBD: Print contents of table
	int i = 0;
	for(i; i < size; i++){
		Node *current = table[i];
		while(current != NULL){
			cout << current->key << "\n";
			current = current->next;
		}
	}
}
