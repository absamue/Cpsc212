#ifndef SEARCH_H /* Prevent multiple inclusion... */
#define SEARCH_H

#include <string>
using namespace std;

struct Page {
	string key;
	int index;
	double weight;
	double new_weight;

	Page *next;
	
	Page(string k, Page *n) { key = k; next = n;}
	Page() { key = ""; weight = -1; new_weight = -1; next = NULL; }
};

struct Word {
	string key;
	int index;

	Word *next;
	Word *pnext;
	
	Word(string k, Word *pn){key = k; pnext = pn;}
	Word(string k, Word *n, int i){key = k; next = n; index = i; pnext =
		NULL;}
};

struct Node{
	string key;
	int index;

	Node *next;
	Node(string k, int v, Node *n) {key = k; index = v; next = n;}
};

class Search {

	private: 
		Page **page_table; // array of pointers to linked lists
		Word **word_table;
		Node **index_table;
		Node **windex_table;
		int size, num_elems;      // size of table, as currently allocated

	public:

		Search(int s);
		~Search();

		//storage
		void insert_index(string key, int val, int check);
		void update_index(void);
		bool find_index(string key, int check);
		int get_index(string key, int check);

		void insert_main(string key, int val);
		void insert_word(string key, string page);
		void insert_wpage(int page, string key);
		void insert_page(string key, int c);
		
		//pagerank
		bool find(Page *p);
		int count_match(Page *current);
		void update(Page *comp, int total);
		void rank(void);
		void pagerank(string s);

		void print(void);
};

#endif
