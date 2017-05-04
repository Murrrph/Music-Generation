/***
 *
 *This file contains the interface and implementation for the list structures we used to represent the rhythm. It contains the probability of 
 *on the same note type, moving up to a longer note, or moving down to a shorter note. It also contains a pointer to the node below and a pointer
 *to the node above. We used a struct containing the probabilities, pointer, and a constructor
 *
 * We also included a class that contains a pointer to the current Listnode of the note being used
 ***/


#include <iostream>
#include <time.h>
using namespace std;

#define BASE_LIST 0	//we had three different lists, one for each of the tracks. This was so the bass and melody had different rhythms
#define MID_LIST 1
#define MELODY_LIST 2

struct ListNode {
	double type; // 2.0 = half note, 1.0 = quarter note, 0.5 = eighth note, 0.25 = sixteenth note
	double p_stay; 	// probabililty of stayings
	double p_down; 	// prob. of going down
	double p_up;	// prob. of going up

	ListNode *down; // pointer to a node below
	ListNode *up;   // pointer to a node above this one

	//constructor for the struct node
	ListNode(double t, double ps, double pd, double pu, ListNode *d, ListNode *u)
	{
		type = t; 
		p_stay = ps; p_down = pd; p_up = pu;
		down = d; up = u;
	}
};

//contains a pointer to the current ListNode structure being used
class NoteTypeList {

	private:
		ListNode *current; // pointer to the current ListNode of the note type being used

	public:
		NoteTypeList(int);//constructor
		~NoteTypeList();	//destructor
		double getNextNote();	//picks the rhythm note type of the next note and returns it as a double

};

// Destructor. Operates by moving down the list and using pointer to delete each of the nodes
NoteTypeList::~NoteTypeList()
{
	if(current) // if not empty
	{
		ListNode *cleanupListNode = current;
		while(cleanupListNode->up) cleanupListNode = cleanupListNode->up;

		ListNode *down = cleanupListNode->down;
		while(down)
		{
			delete cleanupListNode;
			cleanupListNode = down;
			down = cleanupListNode->down;
		}

		delete cleanupListNode;
	}
}

// Constructor, make half, quarter, eigth, and sixteenth note ListNodes
NoteTypeList::NoteTypeList(int list_type)
{
	//"Note" that the potential weights are different depending on which track is being generated
	if(list_type == BASE_LIST)
	{
		ListNode *half = new ListNode(2.0, 0.60, 0.70, 0.0, NULL, NULL);
		ListNode *quarter = new ListNode(1.0, 0.15, 0.0, 0.85, NULL, half);
		half->down = quarter;

		current = half;
	}
	else if(list_type == MID_LIST)
	{
		ListNode *half = new ListNode(2.0, 0.40, 0.60, 0.0, NULL, NULL);
		ListNode *quarter = new ListNode(1.0, 0.60, 0.35, 0.15, NULL, half);
		half->down = quarter;
		ListNode *eighth = new ListNode(0.5, 0.4, 0.0, 0.60, NULL, quarter);
		quarter->down = eighth;

		current = half;
	}
	else if(list_type == MELODY_LIST)
	{
		ListNode *half = new ListNode(2.0, 0.30, 0.70, 0.0, NULL, NULL);
		ListNode *quarter = new ListNode(1.0, 0.5, 0.35, 0.15, NULL, half);
		half->down = quarter;
		ListNode *eighth = new ListNode(0.5, 0.7, 0.10, 0.20, NULL, quarter);
		quarter->down = eighth;
		ListNode *sixteenth = new ListNode(0.25, 0.7, 0.0, 0.3, NULL, eighth);
		eighth->down = sixteenth;

		current = half;
	}
}

// travel list and only pick a note when a random number falls on it's 'stay' probability
double NoteTypeList::getNextNote()
{
	double random = (double)rand() / (double)RAND_MAX;
	while(random > current->p_stay)
	{
		//if(random <= current->p_stay) 	// new
			//return current->type;		// new
		if(random < current->p_stay + current->p_down)
			current = current->down;
		else
			current = current->up;
		random = (double)rand() / (double)RAND_MAX;
	}

	return current->type;
}
