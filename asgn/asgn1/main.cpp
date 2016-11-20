#include <cstring>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "search.h"

using namespace std;

void load(void){
	string s;
	string mainpage;
	int size = -1;
	int count = -1;

	//determine the number of newpages to determine the size of the table
	ifstream fin;
	fin.open("webpages.txt");
	while(fin >> s){
		if(s == "NEWPAGE"){
			size++;
		}	
	}
	fin.close();
	//Create a new search class and allocate the tables according to size
	Search E(size);

	//re-search the file. if a link is found in clemson's domain, insert it
	//into page table. otherwise insert the words found into the word table
	fin.open("webpages.txt");
	while(fin >> s){
		if(s == "NEWPAGE"){
			fin >> s;
			mainpage = s;
			count++;
			E.insert_main(s, count);
			continue;
		}
		else{
			if(s.find("clemson.edu", 0) != std::string::npos){
				E.insert_page(s, count);
			continue;
			}
			else{
				E.insert_word(s, mainpage);
				continue;
			}
		}
	}
	fin.close();

	cout << "\n\nValues loaded into table.\n\n";
	//Set up the outgoing link's indexes and proceed through PageRank
	E.update_index();
	E.rank();
	
	//search engine to find specific word
	cout << "Please enter a search query.\n";
	while (cin >> s){
		if(s == "QUIT")
			break;
		E.pagerank(s);
		cout << "\nPlease enter a search query.\n";
		cout << "Type 'QUIT' to exit search\n\n";
	}
}

int main(void){
	load();
	return 0;
}
