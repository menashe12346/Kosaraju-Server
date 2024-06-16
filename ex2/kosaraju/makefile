CXX = g++
CXXFLAGS = -std=c++17 -O2 -pg # for gprof

all: kosaraju_deque kosaraju_list kosaraju_matrix kosaraju_vector_list kosaraju_linked_list main

kosaraju_linked_list: ../../ex1/kosaraju_linked_list.cpp ../../ex1/kosaraju_linked_list.hpp
	$(CXX) $(CXXFLAGS) -c ../../ex1/kosaraju_linked_list.cpp

kosaraju_deque: kosaraju_deque.cpp kosaraju_deque.hpp
	$(CXX) $(CXXFLAGS) -c kosaraju_deque.cpp

kosaraju_list: kosaraju_list.cpp kosaraju_list.hpp
	$(CXX) $(CXXFLAGS) -c kosaraju_list.cpp

kosaraju_matrix: kosaraju_matrix.cpp kosaraju_matrix.hpp
	$(CXX) $(CXXFLAGS) -c kosaraju_matrix.cpp

kosaraju_vector_list: kosaraju_vector_list.cpp kosaraju_vector_list.hpp
	$(CXX) $(CXXFLAGS) -c kosaraju_vector_list.cpp

main: main.cpp kosaraju_linked_list.o kosaraju_deque.o kosaraju_list.o kosaraju_matrix.o kosaraju_vector_list.o
	$(CXX) $(CXXFLAGS) -o main main.cpp kosaraju_linked_list.o kosaraju_deque.o kosaraju_list.o kosaraju_matrix.o kosaraju_vector_list.o

clean:
	rm -f *.o main analysis.txt
