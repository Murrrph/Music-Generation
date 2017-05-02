#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <map>
#include <math.h>

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
		int octaveDistance;

	public:	
		
		Node * getNextNode();
		Graph();
		void addNode(int note);
		void makeAnEdge(Node * first, Node * second);
		void makeEdges();
		void changeOctave(int delta);
		void normalizeEdgeWeights(Node *);
};

Graph::Graph(){
	size = 0;
	octaveDistance = 0;
}

void Graph::addNode(int number){
	
	Node temp_node;
	temp_node.note = number; 

	all_nodes.push_back(temp_node);
	current_node = &(all_nodes.back());

	size += 1;
}

void Graph::normalizeEdgeWeights(Node *node)
{
	double totalWeight = 0;
	for(auto it=node->edges.begin(); it!=node->edges.end(); it++)
	{
		totalWeight += it->second; // add weight of current node
	}
	
	for(auto it=node->edges.begin(); it!=node->edges.end(); it++)
	{
		it->second = it->second / totalWeight;
	}
}

void Graph::makeAnEdge(Node * first, Node * second)
{
	double numToPush_first = (first->edges.size() != 0) ? 1.0/(first->edges.size()) : 1;
	double numToPush_second = (second->edges.size() != 0) ? 1.0/(second->edges.size()) : 1;

	if(first->note == second->note)
	{
		first->edges.insert(pair<Node*, double>(second, numToPush_first));
	}
	else
	{
		first->edges.insert(pair<Node*, double>(second, numToPush_first));
		second->edges.insert(pair<Node*, double>(first, numToPush_second));
	}
	normalizeEdgeWeights(first);
	normalizeEdgeWeights(second);
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
}

void Graph::changeOctave(int delta)
{
	if(octaveDistance+delta <= 3 && octaveDistance+delta >= -2)
	{
		for(int i = 0; i < all_nodes.size(); i++)
		{
			all_nodes[i].note += delta*12;
		}
		octaveDistance += delta;
	}
}

// randomly selects a node connected to the current node
Node * Graph::getNextNode(){
	double random = (double)rand() / (double)RAND_MAX;

	double counter = 0; 
	for(auto currEdge = current_node->edges.begin(); currEdge != current_node->edges.end(); currEdge++)
	{
		if(random < currEdge->second + counter){
			current_node = currEdge->first;
			return currEdge->first;
		}
		counter += currEdge->second;
	}

	return NULL;
}
