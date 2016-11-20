#ifndef MAPSET_H /* Prevent multiple inclusion... */
#define MAPSET_H

#include <math.h>
using namespace std;

struct Node {
	double x_key, y_key;
	Node *next;
	Node(double x, double y, Node *n) { x_key = x; y_key = y; next =
		n; }
	Node() { x_key = -1; y_key = -1; next = NULL; }
};

class Mapset {

	private: 
		Node ***table;  // array of pointers to linked lists
		int size;      // size of table, as currently allocated
		double lowest;
		Node pair1;
		Node pair2;

	public:
		Mapset();
		~Mapset();
		void insert(double x, double y);
		void calc_dist(Node *point1, Node *point2);
		void compare_current(Node *comp);
		void compare_adjacent(Node *comp1, Node
				*comp2);
		void find_closest();
		void print();
};

#endif
