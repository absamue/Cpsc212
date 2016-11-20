#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <algorithm>
using namespace std;

struct Edge {
	int src, dst;
	float travel_time;
};

#define INF 9999999

int N, M;
Edge *E;
vector<int> first_edge;
typedef pair<float, int> point;
priority_queue<point> Q;

// compare two edges based on their source nodes 
// (break ties using destination nodes)
bool comp(Edge e1, Edge e2)
{
	if (e1.src == e2.src) return e1.dst < e2.dst;
	return e1.src < e2.src;
}


int search(int S){
	//array of maximum distances traveled
	float *dist = new float[N];
	for(int i=0; i<N; i++) dist[i] = INF;
	dist[S] = 0;

	//insert the source first
	Q.push(point(0, S));


	while(!Q.empty()){
		point temp = Q.top();
		temp.first *= -1;
		Q.pop();

		//queue source neighbors if they offer a better distance
		int i = first_edge[temp.second];
		while(E[i].src == temp.second && i < M){
			if(temp.first + E[i].travel_time < dist[E[i].dst]){
				dist[E[i].dst] = temp.first + E[i].travel_time;
				Q.push(point(dist[E[i].dst] * -1, E[i].dst));
			}
			i++;
		}
	}

	//get the largest distance found
	int largest=0;
	for(int i=0; i<N; i++){
		if(dist[i] > largest)
			largest = dist[i];
	}

	return largest;
}

int main(void)
{
	FILE *fp = popen ("zcat /tmp/usa_network.txt.gz", "r");
	fscanf (fp, "%d %d", &N, &M);
	E = new Edge[M];
	printf ("Reading input");
	for (int i=0; i<M; i++) {
		fscanf (fp, "%d %d %f", &E[i].src, &E[i].dst, &E[i].travel_time);
		if (i % 1000000 == 0) { printf ("."); fflush(stdout); }
	}
	fclose(fp);
	printf (" Done.\n" ); fflush(stdout); 

	sort(E, E+M, comp);

	//make an index of node locations in the M size array
	int test = -1;
	int j = 0;
	for(int i=0; i<M; i++){
		if(E[i].src != test){
			test = E[i].src;
			first_edge.push_back(i);
		}
	}


	// The node right in front of McAdams Hall is number 17731931.
	int X = search(17731931);
	cout << "X: " << X << "\n";

	//swap the source and destination pairs and resort
	cout << "Swapping edges.";
	for(int i=0; i<M; i++){
		swap(E[i].src, E[i].dst);
		if(i % 1000000 == 0)
			cout << "." << flush;
	}
	cout << "\nSorting\n";
	sort(E, E+M, comp);

	//rebuild the first-edge index now that the indices have changed
	first_edge.clear();
	test = -1;
	j = 0;
	for(int i=0; i<M; i++){
		if(E[i].src != test){
			test = E[i].src;
			first_edge.push_back(i);
		}
	}

	//search for McAdams Hall in the opposite direction
	int Y = search(17731931);
	cout << "Y: " << Y << "\n";
	cout << "(X+Y)/2: " << (X+Y)/2 << "\n";

	return 0;
}
