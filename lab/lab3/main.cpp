#include <fstream>
#include <iostream>
#include "mapset.h"

using namespace std;

void closest(void){
	Mapset M;
	double x;
	double y;

	ifstream wordfile("points.txt");
	while (wordfile >> x >> y)
		M.insert(x, y);
	wordfile.close();

	cout << "Points loaded, now calculating closest pair. \n\n";
	M.find_closest();
	M.print();
	
}

int main(void)
{
	closest();
	return 0;
}
