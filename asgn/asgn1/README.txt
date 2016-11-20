Andrew Samuels
Comp Sci 212: Algorigthms and data structures
September 21st, 2015
Assignment 1: 

Compiling and execution:
	The program can be run by running:
	g++ -o main main.cpp search.cpp
	and then executing ./main

	Note: The program will not run without the file "webpages.txt" contained
	in the same directory


Project description:
	The goal of this program was to achieve a deeper understanding of the
	applications of hash tables and linked lists, while developing a
	proficiency of creating and writing both. 

	In this program, a miniature web search engine was created. By storing a
	data set of hyperlinks and words, it was possible to create a search
	engine based on Google's PageRank algorithm, allowing fast searches to
	relevant webpages based on a searched word.

Functionality:
	This program will create a index of mainpages and their outgoing links
	in the following fashion:
	
		Mainpage  -> Outgoing Link -> Outging Link -> ...
			|
		Mainpage  -> Outgoing Link -> ...
			|
		Mainpage  -> ...

	Simultaneously during processing the text file, the program creates an
	inverted index of words and the pages that they appear on in similar
	fashion:

		Word  -> Page appeared on -> Page appeared on -> ...
		  |
		Word  -> Page appeared on -> ...
		  |
		Word  -> ...

Non-functioning sections:
	Since at readtime, there is no effective way (that I know of) to collect the number of
	unique words without storing them. Therefore it is highly likely that
	the word's storage will become full and require reallocation.
	
	I was unable to properly create the reallocation function, and to
	essentially force the program to work properly I allocated an
	arbitrarily large inverted index to avoid reaching the maximum size. If
	given a different input file than what was given with the assignment, it
	is likely that the index will fill and the program will segfault after
	trying to insert outside of the index.
	This problem does not occour when only using the given file due to the
	workaround.

	Once the program reaches the deconstructor, all of the storage tables
	are deallocated except for word_table[]. When attempting to delete the
	linked lists in word_table, a seg fault occours when trying to delete
	the lists. At time of writing this, I was unable to solve this error.


	SOLUTION TO THIS PROBLEM (November 2015)
	Using a vector of structs alleviates the need to manually reallocate the
	size of the table, meaning it is possible to insert everything into a
	vector and allow the STL to solve the size of the vector on its own

Notes to self:
	Write down step by step the process ON PAPER. Only thinking about it and
	typing has a high probability of creating inefficient methods and
	requires more rewrites. More rewrites -> more work -> more frustration
	-> things dont work like above.

	Make everything into separate logical classes. Yes this program "fits"
	into one file and works, but it became spread out having multiple
	processes all in one place.

	
