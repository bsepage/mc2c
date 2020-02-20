CXX	= g++
SRC	= src/map.cpp src/config.cpp
CXXFLAGS= -Wall -pedantic -std=c++11 -g
BIN	= receiver sender gettime

all: $(BIN)

receiver:
	$(CXX) $(CXXFLAGS) src/receiver.cpp $(SRC) -o bin/receiver

sender:
	$(CXX) $(CXXFLAGS) src/sender.cpp $(SRC) -o bin/sender

gettime:
	gcc src/gettime.c -o bin/gettime

clean:
	rm -f bin/*
	rm -f sim/*.dat
