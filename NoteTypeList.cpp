#include <iostream>
#include <time.h>
using namespace std;

#define BASE_LIST 0
#define MID_LIST 1
#define MELODY_LIST 2

struct ListNode {
	double type; // 2.0 = half note, 1.0 = quarter note, 0.5 = eighth note, 0.25 = sixteenth note
	double p_stay; 	// probabililty of stayings
	double p_down; 	// prob. of going down
	double p_up;	// prob. of going up

	ListNode *down; // pointer to a node below
	ListNode *up;   // pointer to a node above this one

	ListNode(double t, double ps, double pd, double pu, ListNode *d, ListNode *u)
	{
		type = t; 
		p_stay = ps; p_down = pd; p_up = pu;
		down = d; up = u;
	}
};

class NoteTypeList {

	private:
		ListNode *current; // pointer to the current ListNode of the note type being used

	public:
		NoteTypeList(int);
		~NoteTypeList();
		double getNextNote();

};

// Destructor
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
	//ListNode *origin = 
	//current = origin;

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

/*int main()
{
	srand(time(NULL));

	NoteTypeList k;
	int half = 0, quarter = 0, eighth = 0, sixteenth = 0;
	for(int i=0; i<1000; i++)
	{
		double r = k.getNextNote();
		if(r == 2.0)
			half++;
		else if(r == 1.0)
			quarter++;
		else if(r== 0.5)
			eighth++;
		else
			sixteenth++;
	}
	cout << "half notes: " << half << endl;
	cout << "quarter notes: " << quarter << endl;
	cout << "eighth notes: " << eighth << endl;
	cout << "sixteenth notes: " << sixteenth << endl;
	return 0;
}*/
