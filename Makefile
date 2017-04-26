
OPT:= -g -std=c++11 
INC:=$(INC) -I./ -I/${HOME}/include
CXX:= g++
INSTALLPATH:=$(HOME)/bin
LIBS:= -L/${HOME}/lib -lmidifile

ARGS:= $(OPT) $(INC)
TOMIDI= note_to_midi.o
OBJFILES= $(OTHER)

all : note_to_midi graph

graph: graph.cpp
	g++ -std=c++11 -o graph graph.cpp


note_to_midi : $(TOMIDI)	
	$(CXX) $(ARGS) -o note_to_midi $(OBJFILES) $(TOMIDI) $(LIBS)

%.o : %.cpp
	$(CXX) $(ARGS) -c $*.cpp

clean : 
	rm -f $(OBJFILES) note_to_midi
	rm -f $(OBJFILES) graph
	rm -f $(OBJFILES) *.o

