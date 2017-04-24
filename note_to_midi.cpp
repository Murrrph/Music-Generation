// note_to_midi.cpp
//
// Description:   Demonstration of how to create a Multi-track MIDI file.
//

#include "MidiFile.h"
#include <iostream>
#include "std_lib_facilities.h"

using namespace std;

typedef unsigned char uchar;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
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

   for (unsigned i=0; i<rhythm1.size()-2; i++)
   rhythm1[i] = note_lengths[rand() % 2]; // randomly generate eight, quarter or half note
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
   return 0;
}


/*  FUNCTIONS available in the MidiFile class:

void absoluteTime(void);
   Set the time information to absolute time.
int addEvent(int aTrack, int aTime, vector<uchar>& midiData);
   Add an event to the end of a MIDI track.
int addTrack(void);
   Add an empty track to the MIDI file.
int addTrack(int count);
   Add a number of empty tracks to the MIDI file.
void deltaTime(void);
   Set the time information to delta time.
void deleteTrack(int aTrack);
   remove a track from the MIDI file.
void erase(void);
   Empty the contents of the MIDI file, leaving one track with no data.
MFEvent& getEvent(int aTrack, int anIndex);
   Return a MIDI event from the Track.
int getTimeState(void);
   Indicates if the timestate is TIME_STATE_ABSOLUTE or TIME_STATE_DELTA.
int getTrackState(void);
   Indicates if the tracks are being processed as multiple tracks or
   as a single track.
int getTicksPerQuarterNote(void);
   Returns the ticks per quarter note value from the MIDI file.
int getTrackCount(void);
   Returns the number of tracks in the MIDI file.
int getNumTracks(void);
   Alias for getTrackCount();
int getNumEvents(int aTrack);
   Return the number of events present in the given track.
void joinTracks(void);
   Merge all tracks together into one track.  This function is reversable,
   unlike mergeTracks().
void mergeTracks(int aTrack1, int aTrack2);
   Combine the two tracks into a single track stored in Track1.  Track2
   is deleted.
int read(char* aFile);
   Read the contents of a MIDI file into the MidiFile class data structure
void setTicksPerQuarterNote    (int ticks);
   Set the MIDI file's ticks per quarter note information
void sortTrack(vector<MFEvent>& trackData);
   If in absolute time, sort particular track into correct time order.
void sortTracks(void);
   If in absolute time, sort tracks into correct time order.

*/



