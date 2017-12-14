CPP=g++
LD=g++ 
SPECIALFLAGS=-O2
#SPECIALFLAGS=-g -O0
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)

CFLAGS = $(SPECIALFLAGS) -I.
LFLAGS = -L.

## export LD_LIBRARY_PATH=PileupReweighting/StandAlone:$LD_LIBRARY_PATH

#CFLAGS += -IJetUncertainties -IPileupReweighting
#LFLAGS += -LJetUncertainties/StandAlone -lJetUncertainties -LPileupReweighting/StandAlone -lPileupReweighting

CFLAGS += -IPileupReweighting
LFLAGS += -LPileupReweighting/StandAlone -lPileupReweighting

RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC=d_ana.cpp d.cpp

%.o: %.cpp d.h
	$(CPP) $(RCXX) -c $<

all: $(SRC:.cpp=.o)
	$(LD) $(SRC:.cpp=.o) $(RLXX) -o d_ana

clean:
	rm -f *~ *.o
	rm -f d_ana
