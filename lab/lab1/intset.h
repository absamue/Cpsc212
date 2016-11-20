#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H
struct Node{
	int val;
	Node *next;
};


class Intset {

 private: 
  int size, allocated;
  int *A;
  Node *first;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};


#endif
