#include <cstdlib>
#include <iostream>
#include <string.h>
#include <assert.h>
#include "search.h"

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
Page **allocate_page(int size)
{
	Page **table = new Page *[size];
	for (int i=0; i<size; i++)
		table[i] = NULL;
	return table;
}

Word **allocate_word(int size){
	Word **table = new Word *[size];
	for (int i=0; i<size; i++)
		table[i] = NULL;
	return table;
}

Node **allocate_index(int size){
	Node ** table = new Node *[size];
	for(int i=0; i<size; i++){
		table[i] = NULL;
	}
	return table;
}


Search::Search(int s)
{
	size = s;
	num_elems = 0;
	page_table = allocate_page(size);
	//arbitrary large size that circumvents reallocation
	word_table = allocate_word(size * 10); 
	index_table = allocate_index(size);
	windex_table = allocate_index(size);
}

Search::~Search()
{
	int test, test1;
	for (int i=0; i<size; i++) {
		while (page_table[i] != NULL) {
			Page *temp = page_table[i];
			page_table[i] = page_table[i]->next;
			delete temp;
		}

		while(index_table[i] != NULL){
			Node *temp = index_table[i];
			index_table[i] = index_table[i]->next;
			delete temp;
		}
		while (windex_table[i] != NULL){
			Node *temp = windex_table[i];
			windex_table[i] = windex_table[i]->next;
			delete temp;
		}
	}

	for(int i=0; i<(size * 10); i++){
		if(word_table[i] == NULL)
			break;
		while(word_table[i]->pnext != NULL){
			Word *temp = word_table[i]->pnext;
			word_table[i]->pnext = word_table[i]->pnext->pnext;
			delete temp;
		}
	}


	delete[] page_table;
	delete[] word_table;
	delete[] index_table;
	delete[] windex_table;
}


/*
--------------------------------------------------------------------------
										Data Storage
--------------------------------------------------------------------------
*/

//return true if the mainpage or word is found
bool Search::find_index(string key, int check){
	int h = hash(key, size);
	//searching from page index
	if(check == 0){
		Node *temp = index_table[h];
		while(temp != NULL){
			if(temp->key == key)
				return true;
			temp = temp->next;
		}

		return false;
	}
	//searching from word index
	else{
		Node *temp = windex_table[h];
		while(temp != NULL){
			if(temp->key == key)
				return true;
			temp = temp->next;
		}

		return false;
	}
}

//hashtable to hold where mainpages or words are
void Search::insert_index(string key, int val, int check){
	int h = hash(key, size);

	if(check == 0)
		index_table[h] = new Node(key, val, index_table[h]);
	else
		windex_table[h] = new Node(key, val, windex_table[h]);
}

//get index of where in the respective tables mainpages or words are
int Search::get_index(string key, int check){	
	if(check == 0){
		if(find_index(key, check)){
			Node *temp = index_table[hash(key, size)];
			while(temp->key != key){
				temp = temp->next;
			}
			return temp->index;
		}
		else
			return -1;
	}
	else{
		if(find_index(key, check)){
			Node *temp = windex_table[hash(key, size)];
			while(temp->key != key){
				temp = temp->next;
			}
			return temp->index;
		}
		else
			return -1;
	}
}

//insert new mainpage
void Search::insert_main(string key, int c){
	page_table[c] = new Page(key, page_table[c]);
	word_table[c] = new Word(key, word_table[c]);

	insert_index(key, c, 0);

}

//insert an outgoing link after respective mainpage
void Search::insert_page(string key, int c)
{
	Page *in = new Page;
	in->key = key;
	Page *temp = page_table[c];

	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = in;
}

//insert a word's matching page
void Search::insert_wpage(int page, string key){

	word_table[page]->pnext = new Word(key, word_table[page]->pnext);
}

//insert word into list
void Search::insert_word(string key, string page){
	//found new word, insert into front of a list
	if(!find_index(key, 1)){
		num_elems++;

		word_table[num_elems] = new Word(key, word_table[num_elems], num_elems);
		insert_wpage(num_elems, page);
		insert_index(key, num_elems, 1);

	}
	//word already in table, add page to word's list
	else{
		insert_wpage(get_index(key, 1), page);
	}
}

//set the index of outgoing links to know where matching mainpages are, if
//they exist
void Search::update_index(void){
	for(int i=0; i<size; i++){
		Page *temp = page_table[i]->next;
		while(temp != NULL){
			temp->index = get_index(temp->key, 0);
			temp = temp->next;
		}
	}
}

void Search::print(void){
	//test conditions	
}


/*
------------------------------------------------------------------------------
											PageRank
------------------------------------------------------------------------------
*/

//check if page is a mainpage
bool Search::find(Page *p){
	if(p->index != -1)
		return true;
	else
		return false;
}

//find total number of linked to pages (t of pagerank calculation)
int Search::count_match(Page *current){
	int c = 0;
	Page *temp = current->next;

	while(temp != NULL){
		if(find(temp))
			c++; //punny
		temp = temp->next;
	}
	return c;
}

//calculate the new_weights
void Search::update(Page *comp, int total){
	Page *temp = comp->next;
	while(temp != NULL){
		if(find(temp)){
			page_table[temp->index]->new_weight += (0.9 * comp->weight) /
				(double)total;
		}
		temp = temp->next;
	}
}

//page rank calculation
void Search::rank(void){
	int i, j;
	int count = 0;
	//set an inital weight of all the pages to be equally distributed
	for(i=0; i<size; i++){
		Page *temp = page_table[i];
		temp->weight = (1 / (double)size);
	}

	//Random walk to set up page rank
	while(count < 50){
		//initialize new weight to uniform distribution
		for(i=0; i<size; i++){
			Page *temp = page_table[i];
			page_table[i]->new_weight = (0.1 / (double)size);
		}

		//find if there is a matching page, and if so update the weight
		for(i=0; i<size; i++){
			Page *temp = page_table[i];	
			int total = count_match(temp);
			update(temp, total);
		}	

		//set weights to new weight
		for(i=0; i<size; i++){
			page_table[i]->weight = page_table[i]->new_weight;
		}

		//The "why is it taking so long?" important step
		count++;
	}
	cout << "Ranking completed\n\n";
}

/*
---------------------------------------------------------------------------------------------
												Search Engine
---------------------------------------------------------------------------------------------
*/

//prints out the queried word's matching pages and respective pageranks
void Search::pagerank(string s){
	int w, p;
	w = get_index(s, 1);
	if(w != -1){
		Word *temp = word_table[w]->pnext;
		while(temp != NULL){
			p = get_index(temp->key, 0);
			if( p != -1){
				Page *ptemp = page_table[p];
				cout << (int)(ptemp->weight * 10000000) << " " << ptemp->key << "\n";
			}
			temp = temp->pnext;
		}

		cout << "\nSearch completed\n";
	}
	else
		cout << "No results found\n";
}
