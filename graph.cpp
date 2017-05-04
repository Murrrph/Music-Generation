/***
 * This file contains the implementation of our graph data structure, which is use to connections between the notes. Accordingly, each of the nodes contains a note value along with weighted connections to other nodes. This way, a simple graph traversal will create a sequence of notes to be used in the song.
 *
 ***/

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

//each note is connected to the two notes above and two notes below
static int NUM_CONNECTED = 2;

struct Node{

	int note; //C=60
	map<Node*, double> edges; //adjacency list

};


class Graph{
	
	private:
		vector<Node> all_nodes;
		int size;	//nodes in graph	
		Node * current_node; //pointer to current node
		int octaveDistance;	//number of octaves away from the start

	public:	
		
		Node * getNextNode();//gets the next node in the song using the weighted probabilities
		Graph();
		void addNode(int note);	//adds a node to the graph
		void makeAnEdge(Node * first, Node * second);	//makes an edge between two nodes
		void makeEdges();	//uses the makeAnEdge function to make all of the appropriate edges
		void changeOctave(int delta); 	//changes the octave of the notes
		void normalizeEdgeWeights(Node *);	//normalizes the edge weights so that they add to one
};

//Constructor for graph
Graph::Graph(){
	size = 0;
	octaveDistance = 0;
}

//takes a number and creates a node with that note value by adding to the vector of nodes
void Graph::addNode(int number){
	
	Node temp_node;
	temp_node.note = number; 

	all_nodes.push_back(temp_node);	
	current_node = &(all_nodes.back()); //sets the current node pointer to a ponter to the end of all the nodes

	size += 1;
}

//Takes in a node as a parameter, and normalizes all of the edge weights so that they add to 1. Basically adds the weight of the current node and then divides by the total at the end. Note that this works both when weights are first generated at the beginning and when weights are changed
void Graph::normalizeEdgeWeights(Node *node)
{
	double totalWeight = 0;
	for(auto it=node->edges.begin(); it!=node->edges.end(); it++)
	{
		totalWeight += it->second; // add weight of current node
	}
	
	for(auto it=node->edges.begin(); it!=node->edges.end(); it++)
	{
		it->second = it->second / totalWeight;	//by doing this to all nodes, we normalize them
	}
}

//Creates an edge between two nodes by inserting a pair making up of a node pointer and the appropriate value
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
	normalizeEdgeWeights(first);	//at the end we normalize both of the nodes involved
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
//If the song is within a certain distance of the starting point, then this function changes the octave
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

//Randomly selects a node connected to the current node based on the weights. Basically generates a random number and iterates through the weights until it finds one that is just above the sum of the previous probabilities
Node * Graph::getNextNode(){
	double random = (double)rand() / (double)RAND_MAX;

	double counter = 0; 
	for(auto currEdge = current_node->edges.begin(); currEdge != current_node->edges.end(); currEdge++)
	{
		if(random < currEdge->second + counter){
			current_node = currEdge->first;
			return currEdge->first;	//once we find the selected node, we return the pointer
		}
		counter += currEdge->second;	//we update the counter so we can test to see if the next node was chosen
	}

	return NULL;
}
