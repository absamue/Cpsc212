#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#define INF 99999999
using namespace std;

typedef pair<int, string> Point;

struct State{
	string name;
	int dist;
	vector<string> neighbors;
	vector<int> ecc;
	int ec;
};

map<string, State *> Nodes;

//breadth first search
void search(State *S){
	queue<State*> Q;
	Q.push(S);

	State *temp;

	while(!Q.empty()){
		temp = Q.front();
		Q.pop();
		for(int i=0; i<temp->neighbors.size(); i++){
			if(Nodes[temp->neighbors[i]]->dist == INF){
				Nodes[temp->neighbors[i]]->dist = temp->dist + 1;
				Q.push(Nodes[temp->neighbors[i]]);
			}
		}
	}
}

int main(void){
	string S, N;

	//read in all of the states in both directions
	ifstream wordfile("usa_48_state_graph.txt");
	while(wordfile >> S >> N){
		if(Nodes.count(S) <= 0){
			Nodes[S] = new State;
			Nodes[S]->name = S;
			Nodes[S]->neighbors.push_back(N);
		}
		else{
			Nodes[S]->neighbors.push_back(N);
		}

		if(Nodes.count(N) <= 0){
			Nodes[N] = new State;
			Nodes[N]->name = N;
			Nodes[N]->neighbors.push_back(S);
		}
      else{
			Nodes[N]->neighbors.push_back(S);
		}

	}

	map<string, State *>::iterator it;
	map<string, State *>::iterator fix;
	//breadth search every state
	for(it = Nodes.begin(); it != Nodes.end(); it++){
		//set all states as unvisited
		for(fix = Nodes.begin(); fix!= Nodes.end(); fix++){
			fix->second->dist = INF;
		}
		//set the current distance to zero
		it->second->dist = 0;
		search(it->second);

		//get all of the distances and then pick the eccentric
		for(fix = Nodes.begin(); fix!=Nodes.end(); fix++){
			State *temp = fix->second;
			it->second->ecc.push_back(temp->dist);
		}
		sort(it->second->ecc.begin(), it->second->ecc.end());
		it->second->ec = it->second->ecc.back();
	}

	//set up a structure of each state and it's eccentricity
	vector<Point> V;
	for(it = Nodes.begin(); it!= Nodes.end(); it++){
		V.push_back(Point(it->second->ec, it->second->name));
	}
	sort(V.begin(), V.end());

	for(int i=0; i<V.size(); i++){
		cout << V[i].first << " " << V[i].second << "\n";
	}
	
	return 0;
}
