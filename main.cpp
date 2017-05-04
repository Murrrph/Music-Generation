/*
 *This file contains the implementation for each of the scales, generates the rhythm using functions from the NoteTypeList class, generates the
 *notes using functions and datatypes from graph.cpp, and uses the Midifile library to write a midifile. We also have code that
 *varies the volume of the music and uses the first 10 notes of the song to vary the weights between the notes, creating a melody throughout the 
 *song
 */

#include <iostream>
#include <fstream>
#include "MidiFile.h"
#include "graph.cpp"
#include "NoteTypeList.cpp"
using namespace std;

typedef unsigned char uchar;

int octaveDiff = 12;


//Assigns integers to different note values depending on the MIDI standard
int C = 60;
int CS = 61; // C Sharp
int D = 62;
int DS = 63; // D Sharp
int E = 64;
int F = 65;
int FS = 66; // F Sharp
int G = 67;
int GS = 68; // G Sharp
int A = 69;
int AS = 70; // A Sharp
int B = 71;

int main()
{
	srand(time(NULL));
	//construct various scales to be used later in the program
	Graph C_scale;
	C_scale.addNode(C - octaveDiff);
	C_scale.addNode(D - octaveDiff);
	C_scale.addNode(E - octaveDiff);
	C_scale.addNode(F - octaveDiff);
	C_scale.addNode(G - octaveDiff);
	C_scale.addNode(A - octaveDiff);
	C_scale.addNode(B - octaveDiff);
	C_scale.addNode(C);
	C_scale.makeEdges();

	Graph D_scale;
	D_scale.addNode(CS);
	D_scale.addNode(D - octaveDiff);
	D_scale.addNode(E - octaveDiff);
	D_scale.addNode(FS - octaveDiff);
	D_scale.addNode(G - octaveDiff);
	D_scale.addNode(A - octaveDiff);
	D_scale.addNode(B - octaveDiff);
	D_scale.addNode(D);
	D_scale.makeEdges();

	Graph E_scale;
	E_scale.addNode(CS);
	E_scale.addNode(DS);
	E_scale.addNode(E - octaveDiff);
	E_scale.addNode(FS - octaveDiff);
	E_scale.addNode(GS - octaveDiff);
	E_scale.addNode(A - octaveDiff);
	E_scale.addNode(B - octaveDiff);
	E_scale.addNode(D);
	E_scale.makeEdges();

	Graph F_scale;
	F_scale.addNode(C);
	F_scale.addNode(D);
	F_scale.addNode(E);
	F_scale.addNode(F - octaveDiff);
	F_scale.addNode(G - octaveDiff);
	F_scale.addNode(A - octaveDiff);
	F_scale.addNode(AS - octaveDiff);
	F_scale.addNode(F);
	F_scale.makeEdges();

	Graph G_scale;
	G_scale.addNode(C);
	G_scale.addNode(D);
	G_scale.addNode(E);
	G_scale.addNode(FS);
	G_scale.addNode(G - octaveDiff);
	G_scale.addNode(A - octaveDiff);
	G_scale.addNode(B - octaveDiff);
	G_scale.addNode(G);
	G_scale.makeEdges();

	Graph A_scale;
	A_scale.addNode(CS);
	A_scale.addNode(D);
	A_scale.addNode(E);
	A_scale.addNode(FS);
	A_scale.addNode(GS);
	A_scale.addNode(A - octaveDiff);
	A_scale.addNode(B - octaveDiff);
	A_scale.addNode(A);
	A_scale.makeEdges();

	Graph B_scale;
	B_scale.addNode(CS);
	B_scale.addNode(DS);
	B_scale.addNode(E);
	B_scale.addNode(FS);
	B_scale.addNode(GS);
	B_scale.addNode(AS);
	B_scale.addNode(B - octaveDiff);
	B_scale.addNode(B);
	B_scale.makeEdges();

	Graph someScale;
	someScale.addNode(AS - octaveDiff);
	someScale.addNode(C);
	someScale.addNode(D);
	someScale.addNode(DS);
	someScale.addNode(F);
	someScale.addNode(G);
	someScale.makeEdges();

	vector<int> notes_in_song; //vector of notes in the song, in sequential order

	vector<Node *> melody;//vector of node pointers used to construct the melody

	Graph *scaleGraphToUse = &someScale;	//pointer to the scale to use

	//pushes back an initial 10 nodes to be used to seed the melody. Also stores the pointers so we can easily access the nodes later to change their probabilities
	for(int i = 0; i < 10; i++){
		
		Node * pointer = scaleGraphToUse->getNextNode();
			
		melody.push_back(pointer);
		
		int number = pointer->note;
		//cout << number << endl;
		notes_in_song.push_back(number);

	}
	
	for(int i = 0; i < melody.size()-1; i++){
		melody[i]->edges[melody[i+1]] += .5; //Sorry for obfusction, but this accesses a pointer in the melody vector, accesses the map that is within the node, and uses the next node pointer to access the weight value between the two nodes. Then, we increment the value to increase the weighting, so that the given note sequence is more likely. This makes it so the songs has a recognizable melody
		scaleGraphToUse->normalizeEdgeWeights(melody[i]);	//then normalizes the weights so they add to 1
	}
	//Generates the rest of the notes in the song now that the melody has been seeded. 
	for(int j = 0; j < 120; j++){
		// change octave possible
		if((double)rand() / (double)RAND_MAX < 0.05)
			scaleGraphToUse->changeOctave(1);
		else if((double)rand() / (double)RAND_MAX < 0.05)
			scaleGraphToUse->changeOctave(-1);

		Node * pointer = scaleGraphToUse->getNextNode();
		int number = pointer->note;
		//cout << number << endl;
		notes_in_song.push_back(number);
	}

	//Outputs all of the notes for debugging purposes
	ofstream ost;
	ost.open("notes.txt");
	for(int i = 0; i < notes_in_song.size(); i++){
		int temp;
		temp = notes_in_song[i];
		ost << temp << endl << temp << endl << temp << endl;
	}
	int sentinel = -1;
	ost << sentinel << endl << sentinel << endl << sentinel << endl;
	ost.close();

//NOTE: Some of the code in the following section has been adapted from code written in the Midifile library. Thanks Craig Sapp for your work.


	// -----------  CONVERT TO MIDI SECTION ----------

	MidiFile outputfile;        // create an empty MIDI file with one track
   	outputfile.absoluteTicks();  // time information stored as absolute time
                               // (will be coverted to delta time when written)
   	outputfile.addTrack(3);     // Add another three tracks to the MIDI file
   	vector<uchar> midievent;     // temporary storage for MIDI events
   	midievent.resize(3);        // set the size of the array to 3 bytes
   	int tpq = 120;              // default value in MIDI file is 48
   	outputfile.setTicksPerQuarterNote(tpq);

   	ifstream ist {"notes.txt"};
   	vector<int> note_values;
   	int tmp;
   	while (ist >> tmp) {
		note_values.push_back(tmp);
   	}
	
	//We have three different voices
   	vector<int> voice1;
   	vector<int> voice2;
   	vector<int> voice3;

	//iterates through the note_value vector and adds the notes to the voices. The iterate at different rates so that the baseline is relatively constant while the melody varies more
   	for (unsigned i=0; i<note_values.size(); i+=1)
   		voice1.push_back(note_values[i]);

   	for (unsigned i=0; i<note_values.size(); i+=2)
   		voice2.push_back(note_values[i]);

   	for (unsigned i=0; i<note_values.size(); i+=3)
   		voice3.push_back(note_values[i]+12);		// THE +12 PUTS IT AT AN OCTAVE HIGHER

   	voice3[voice3.size()-1] = -1; // NOTE: voice3 was put at an octave higher (+12) so it's sentinel -1 became 11

   	vector<double> rhythm1(voice1.size());
   	vector<double> rhythm2(voice2.size());
   	vector<double> rhythm3(voice3.size());
   	//vector<double> note_lengths{0.5, 1.0, 2.0};

   	NoteTypeList *ntl = new NoteTypeList(BASE_LIST);
   	for (unsigned i=0; i<rhythm1.size()-2; i++)
   		rhythm1[i] = ntl->getNextNote(); // randomly generate a note type (half, quarter, etc...)

   	NoteTypeList *ntl_mid = new NoteTypeList(MID_LIST);
   	for (unsigned i=0; i<rhythm2.size()-2; i++)
   		rhythm2[i] = ntl_mid->getNextNote();

   	NoteTypeList *ntl_melody = new NoteTypeList(MELODY_LIST);
   	for (unsigned i=0; i<rhythm3.size()-2; i++)
   		rhythm3[i] = ntl_melody->getNextNote();

   	rhythm1[rhythm1.size()-2] = 4.0;  // end on whole note
   	rhythm1[rhythm1.size()-1] = -1.0; // -1 to stop reading
   	rhythm2[rhythm2.size()-2] = 4.0;
   	rhythm2[rhythm2.size()-1] = -1.0;
   	rhythm3[rhythm3.size()-2] = 4.0;
   	rhythm3[rhythm3.size()-1] = -1.0;



   	// store a base line in track 3
   	int i=0;
   	int actiontime = 0;          // reset time for beginning of file
   	midievent[2] = 64;
   	while (voice3[i] > 0) {
      	midievent[0] = 0x90;
      	midievent[1] = voice3[i];

      	cout << voice3[i] << endl;

      	midievent[2] += (rand() % 10) - 5;			// change note velocity (really the volume)
      	if(rand() % 100 < 3)
      		midievent[2] += (rand() % 30) - 15;
      	if(midievent[2] < 30)
      		midievent[2] = 60;
      	if(midievent[2] > 90)
      		midievent[2] = 70;
      	outputfile.addEvent(3, actiontime, midievent);
      	actiontime += tpq * rhythm3[i];
      	midievent[0] = 0x80;
      	outputfile.addEvent(3, actiontime, midievent);
      	i++;
   	}
   	int maxActionTime = actiontime;

   	// store a base line in track 2
   	i=0;
   	actiontime = 0;          // reset time for beginning of file
   	midievent[2] = 64;
   	while (voice2[i] > 0  && actiontime < maxActionTime) {
      	midievent[0] = 0x90;
      	midievent[1] = voice2[i];
      	midievent[2] += (rand() % 10) - 5;
      	
	//this part of the code varies the volume by accessing midievent[2]. This is repeated for each of the tracks, which are varied differently
	if(rand() % 100 < 3)
      		midievent[2] += (rand() % 30) - 15;
      	if(midievent[2] < 30)
      		midievent[2] = 60;
      	if(midievent[2] > 90)
      		midievent[2] = 70;
      	outputfile.addEvent(2, actiontime, midievent);
      	actiontime += tpq * rhythm2[i];
      	midievent[0] = 0x80;
      	outputfile.addEvent(2, actiontime, midievent);
      	i++;
   	}

   	// store a melody line in track 1 (track 0 left empty for conductor info)
   	i=0;
   	actiontime = 0;      // temporary storage for MIDI event time
   	midievent[2] = 64;       // store attack/release velocity for note command
   	while (voice1[i] > 0 && actiontime < maxActionTime) {
     	midievent[0] = 0x90;     // store a note on command (MIDI channel 1)
      	midievent[1] = voice1[i];
      	midievent[2] += (rand() % 6) - 3;
      	if(midievent[2] < 30)
      		midievent[2] = 60;
      	if(midievent[2] > 90)
      		midievent[2] = 70;
      	outputfile.addEvent(1, actiontime, midievent);
      	actiontime += tpq * rhythm1[i];
      	midievent[0] = 0x80;     // store a note on command (MIDI channel 1)
      	outputfile.addEvent(1, actiontime, midievent);
      	i++;
   	}

   	outputfile.sortTracks();         // make sure data is in correct order
   	outputfile.write("song.mid"); // write Standard MIDI File

}
