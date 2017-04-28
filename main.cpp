#include <iostream>
#include <fstream>
#include "MidiFile.h"
#include "graph.cpp"
#include "NoteTypeList.cpp"
using namespace std;

typedef unsigned char uchar;

int octaveDiff = 12;

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

	vector<int> notes_in_song;

	for(int j = 0; j < 60; j++){
		// change octave possible
		if((double)rand() / (double)RAND_MAX < 0.05)
			someScale.changeOctave(1);
		else if((double)rand() / (double)RAND_MAX < 0.05)
			someScale.changeOctave(-1);

		Node * pointer = someScale.getNextNode();
		int number = pointer->note;
		cout << number << endl;
		notes_in_song.push_back(number);
	}
	
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

   	vector<int> voice1;
   	vector<int> voice2;
   	vector<int> voice3;

   	for (unsigned i=2; i<note_values.size(); i+=3)
   		voice1.push_back(note_values[i]);

   	for (unsigned i=1; i<note_values.size(); i+=3)
   		voice2.push_back(note_values[i]);

   	for (unsigned i=0; i<note_values.size(); i+=3)
   		voice3.push_back(note_values[i]);

   	vector<double> rhythm1(voice1.size());
   	vector<double> note_lengths{0.5, 1.0, 2.0};

   	NoteTypeList ntl;
   	for (unsigned i=0; i<rhythm1.size()-2; i++)
   		rhythm1[i] = ntl.getNextNote(); // randomly generate eight, quarter or half note
   	
   	rhythm1[rhythm1.size()-2] = 4.0;  // end on whole note
   	rhythm1[rhythm1.size()-1] = -1.0; // -1 to stop reading

   	vector<double> rhythm2 = rhythm1;
   	vector<double> rhythm3 = rhythm1;

   	// store a melody line in track 1 (track 0 left empty for conductor info)
   	int i=0;
   	int actiontime = 0;      // temporary storage for MIDI event time
   	midievent[2] = 64;       // store attack/release velocity for note command
   	while (voice1[i] >= 0) {
     	midievent[0] = 0x90;     // store a note on command (MIDI channel 1)
      	midievent[1] = voice1[i];
      	outputfile.addEvent(1, actiontime, midievent);
      	actiontime += tpq * rhythm1[i];
      	midievent[0] = 0x80;     // store a note on command (MIDI channel 1)
      	outputfile.addEvent(1, actiontime, midievent);
      	i++;
   	}

   	// store a base line in track 2
   	i=0;
   	actiontime = 0;          // reset time for beginning of file
   	midievent[2] = 64;
   	while (voice2[i] >= 0) {
      	midievent[0] = 0x90;
      	midievent[1] = voice2[i];
      	outputfile.addEvent(2, actiontime, midievent);
      	actiontime += tpq * rhythm2[i];
      	midievent[0] = 0x80;
      	outputfile.addEvent(2, actiontime, midievent);
      	i++;
   	}

   	// store a base line in track 3
   	i=0;
   	actiontime = 0;          // reset time for beginning of file
   	midievent[2] = 64;
   	while (voice3[i] >= 0) {
      	midievent[0] = 0x90;
      	midievent[1] = voice3[i];
      	outputfile.addEvent(3, actiontime, midievent);
      	actiontime += tpq * rhythm3[i];
      	midievent[0] = 0x80;
      	outputfile.addEvent(3, actiontime, midievent);
      	i++;
   	}

   	outputfile.sortTracks();         // make sure data is in correct order
   	outputfile.write("song.mid"); // write Standard MIDI File

}
