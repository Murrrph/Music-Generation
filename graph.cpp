#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <map>

using namespace std;

static int NUM_CONNECTED = 2;

struct Node{

	int note; //C=60
	map<Node*, double> edges; //adjacency list

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
	current_node = &(all_nodes.back());

	size += 1;
}


void Graph::makeAnEdge(Node * first, Node * second){
	if(first->note == second->note)
	{
		first->edges.insert(pair<Node*, double>(second, 1.0));
	}
	else
	{
		first->edges.insert(pair<Node*, double>(second, 1.0));
		second->edges.insert(pair<Node*, double>(first, 1.0));
	}
}

// connect each node to (2) before it and (2) after it
// algorithm: travel nodes array and connect each node to two itself and two nodes in front of it. Because the
//  makeAnEdge function conects the nodes both ways, this means nodes get connected to two nodes before them as well
void Graph::makeEdges(){	
	for(int i = 0; i < all_nodes.size(); i++)
	{
		for(int count=0; count<=NUM_CONNECTED; count++)
		{
			if(i+count >= 0 && i+count < all_nodes.size())	// check if index is in bounds
				makeAnEdge(&all_nodes[i], &all_nodes[i+count]); // connect a node to another node
		}
	}

	// for testing: outputs nodes and their edges
	/*for(int i = 0; i < all_nodes.size(); i ++)
	{
		cout << all_nodes[i].note << ":" << all_nodes[i].edges.size() << endl;
		for(int j=0; j<all_nodes[i].edges.size(); j++)
			cout << "  " << all_nodes[i].edges[j]->note << ", ";
		cout << endl;
	}*/
}

Node Graph::getNode(int index){
	return all_nodes[index];
}

int Graph::getSize(){
	return all_nodes.size();
}

// randomly selects a node connected to the current node
Node * Graph::getNextNode(){
	double random = (double)rand() / (double)RAND_MAX;

	

	int counter = 0; 
	for(auto currEdge = current_node->edges.begin(); currEdge != current_node->edges.end(); currEdge++){

		if(random < currEdge->second + counter){
			return currEdge->first;
		}
		counter += currEdge->second;

	}
	return NULL;
}
