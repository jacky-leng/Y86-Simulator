CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -lpthread -ljsoncpp

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp $(LDFLAGS)

clean:
	rm -f main