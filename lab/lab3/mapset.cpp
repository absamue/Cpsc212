#include <iostream>
#include <assert.h>
#include "mapset.h"

using namespace std;

/* Return a hash for the double x in the range 0..table_size-1 */
int hash(double x, int table_size)
{
	int h = (int)(x * table_size);
	return h;
}


/* Allocate a table of pointers to nodes, all initialized to NULL */
Node ***allocate_table(int size)
{
	//for each element in an array of size, create a second dimension array
	//in each index
	Node ***table = new Node **[size];
	for(int i = 0; i<size; i++)
		table[i] = new Node *[size];
	return table;
}


/*
	For this lab, I chose a grid length of 100. A grid length of 1000 would
	result in 1m total grid locations making it highly likely for a value to be
	the only point in its location. My calculation function is reliant on there
	being two separate points to compare, and as such is entirely reliant on
	collisions. Thus I chose a size smaller than 1m so that I would force
	collisions to happen
 */
Mapset::Mapset()
{
	lowest = 99; //arbitrary large value to mark start
	size = 100; 
	table = allocate_table(size);
}

Mapset::~Mapset()
{
	for (int i=0; i<size; i++) {
		for(int j=0; j<size; j++){
			while (table[i][j] != NULL) {
				Node *temp = table[i][j];
				table[i][j] = table[i][j]->next;
				delete temp;
			}
		}
	}

	for(int i=0; i<size; i++){
		delete []table[i];
	}

	delete []table;
}


/* Inserts a new key.  It is an error if key is already in the set. */
void Mapset::insert(double x, double y)
{  // TBD: Insert new element
	int xh = hash(x, size);
	int yh = hash(y, size);
	table[xh][yh] = new Node(x, y, table[xh][yh]);
}


//distance calculation
void Mapset::calc_dist(Node *point1, Node *point2)
{
	double dist;
	dist = sqrt(pow((point1->x_key - point2->x_key), 2) +
			pow((point1->y_key - point2->y_key), 2));

	if(dist < lowest){
		lowest = dist;
		pair1.x_key = point1->x_key;
		pair1.y_key = point1->y_key;

		pair2.x_key = point2->x_key;
		pair2.y_key = point2->y_key;

	}

}


//find closest of single node
void Mapset::compare_current(Node *comp){

	Node *temp1, *temp2;
	temp1 = comp;
	temp2 = temp1->next;

	while(temp2 != NULL){
		while(temp2 != NULL){
			calc_dist(temp1, temp2);			

			temp2 = temp2->next;
		}
		temp1 = temp1->next;
		temp2 = temp1->next;
	}

}

//find closest pair between two separate nodes
void Mapset::compare_adjacent(Node *comp1, Node *comp2){

	Node *temp1 = comp1;
	Node *temp2 = comp2;

	while(temp1 !=NULL){
		while(temp2 != NULL){

			calc_dist(temp1, temp2);

			temp2 = temp2->next;
		}
		temp1 = temp1->next;
		temp2 = comp2;
	}
}

void Mapset::find_closest(void){
	//check each index in grid
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			//check inside of current
			compare_current(table[i][j]);
			//compare nodes around current
			//"it keeps going"
			for(int a = (j-1); a<(j+1); a++){
				for(int b=(i-1); b<(i+1); b++){
					//ignore current and nodes outside grid
					if(a == j && b == i);
						continue;

					if(a < 0 || a > size)
						continue;
					else if(b < 0 || b > size)
						continue;

					compare_adjacent(table[b][a], table[i][j]);
				}
			}  
		}
	}

}

//print the two closest pairs
void Mapset::print(void)
{
	cout << "The two closest values are:\n";
	cout << "(" << pair1.x_key << ", " << pair1.y_key << ")";
	cout << " and " << "(" << pair2.x_key << ", " << pair2.y_key << ")"
		<<"\n\n";
	cout << "The lowest distance is: " << lowest << "\n";
}
