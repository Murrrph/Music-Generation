#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <fstream>

using namespace std;

struct Node{

	int note; //C=60
	vector<Node*> edges; //adjacency list

};


class Graph{
	
	private:
		vector<Node> all_nodes;
		int size;	
		Node * current_node; //pointer to current node


	public:	
		
		Node * getNextNode();
		Graph();
		void addNode(int note);
		void makeAnEdge(Node * first, Node * second);
		void makeEdges();
		Node getNode(int index); //use .note
		int getSize();//just returns using all_notes.size
};

Graph::Graph(){
	size = 0;

}

void Graph::addNode(int number){
	
	Node temp_node;
	temp_node.note = number; 
	all_nodes.push_back(temp_node);

	size += 1;
	
}


void Graph::makeAnEdge( Node * first, Node * second){
	//later: error checking that the indexed node exists
	//all_nodes[first].edges.push_back(&all_nodes[second]);	
	
	first->edges.push_back(second);
	second->edges.push_back(first);
	

}

void Graph::makeEdges(){
	//for now, we are connecting the nodes to all of the other nodes		
	
	//listy[first].push_back(second);	

	for(int i = 0; i < size; i++){
		for(int j = i; j < (size); j++){
			Node * pointer_first = &all_nodes[i];
			Node * pointer_second = &all_nodes[j];
			makeAnEdge(pointer_first, pointer_second);
		}
	}
}


Node Graph::getNode(int index){
	return all_nodes[index];

}

int Graph::getSize(){

	return all_nodes.size();
}

Node * Graph::getNextNode(){

	//randomly selects the next node
	
	//srand(time(NULL));

	int random = rand() % size;	
	
	//temp way to pick notes
	return &all_nodes[random];
	

}

main(){

srand(time(NULL));

Graph g;
for(int i = 60; i < 68; i++){
	g.addNode(i);
}
g.makeEdges();

vector<int> notes_in_song;

for(int j = 0; j < 60; j++){
	Node * pointer = g.getNextNode();
	int number = pointer->note;
	notes_in_song.push_back(number);
}
/*
for(int k = 0; k < 50; k++){
	cout << notes_in_song[k] << endl;
}
*/

ofstream ost{"notes.txt"};
for(int i = 0; i < notes_in_song.size(); i++){
	int temp;
	temp = notes_in_song[i];
	ost << temp << endl << temp << endl << temp << endl;

}
int sentinel = -1;
ost << sentinel << endl << sentinel << endl << sentinel << endl;
//ost << sentinel << endl;	
}
