OPT:= -g -std=c++11 
CXX:= g++
LIBS:= -L/${HOME}/lib -lmidifile
ARGS:= $(OPT) $(INC)

TOMIDI= note_to_midi.o
OBJFILES= $(OTHER)



all : note_to_midi graph

note_to_midi : note_to_midi.o	
	g++ -g -std=c++11 $(INC) -o note_to_midi $(OBJFILES) note_to_midi.o $(LIBS)

graph: graph.cpp
	g++ -std=c++11 -o graph graph.cpp

%.o : %.cpp
	$(CXX) $(ARGS) -c $*.cpp

clean : 
	rm -f $(OBJFILES) note_to_midi
	rm -f $(OBJFILE) graph
