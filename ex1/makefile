CXX = g++
CXXFLAGS = -std=c++17 -O2

all: main

main: main.o kosaraju_linked_list.o
	$(CXX) $(CXXFLAGS) -o main main.o kosaraju_linked_list.o

main.o: main.cpp kosaraju_linked_list.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

kosaraju_linked_list.o: kosaraju_linked_list.cpp kosaraju_linked_list.hpp
	$(CXX) $(CXXFLAGS) -c kosaraju_linked_list.cpp

clean:
	rm -f *.o main
