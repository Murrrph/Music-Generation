OPT:= -g -std=c++11 
INC:=$(INC) -I./ -I/${HOME}/midifile/include
CXX:= g++
INSTALLPATH:=$(HOME)/midifile/bin
LIBS:= -L/${HOME}/midifile/lib -lmidifile

ARGS:= $(OPT) $(INC)
TOMIDI= note_to_midi.o
OBJFILES= $(OTHER)

all : main

main.o: graph.cpp main.cpp NoteTypeList.cpp
	g++ -g -std=c++11 $(INC) -c main.cpp

main: main.o
	g++ -g -std=c++11 $(INC) -o main main.o $(LIBS)

#note_to_midi : $(TOMIDI)	
	#$(CXX) $(ARGS) -o note_to_midi main.o $(LIBS)

#%.o : %.cpp
	#$(CXX) $(ARGS) -c main.cpp

clean : 
	rm *.o
