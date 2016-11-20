#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>
#include <math.h>
using namespace std;

typedef pair<double, double> Point;
vector<Point> V;
set<Point> S;

//read in everything
void read(void){
	ifstream wordfile("points.txt");

	Point p;
	while(wordfile >> p.first >> p.second){
		V.push_back(p);
	}

	//sort on X
	sort(V.begin(), V.end());
}

//distance calculation
double dist(Point p1, Point p2){
	double d;
	d=sqrt(pow((p2.first-p1.first), 2) + pow((p2.second-p1.second), 2));

	return d;
}

void sweep(void){
	double D = 10;
	int i = 0;
	double test;

	for(int j=0; j<V.size(); j++){
		//insert current into set
		S.insert(Point(V[j].second, V[j].first)); 
		//remove things outside bounds and update i
		while(V[j].first - V[i].first > D){
			S.erase(Point(V[i].second, V[i].first));
			i++;
		}

		//go to the bottom of the available points, and compare each of them
		//to the current one, if it is less than the min, update the min
		//which will also update the bounds
		set<Point>::iterator it;
		it = S.find(Point(V[j].second, V[j].first));
		while(it != S.begin()) it--;
		while(it != S.end()){
			test = dist(Point(it->second, it->first), V[j]);
			if(test < D && test != 0) D = test;
			it++;
		}
	}

	cout << D << "\n";
}

int main(void){
	read();
	sweep();

}
