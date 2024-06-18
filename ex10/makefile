# Makefile for building the server and client applications

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g
LDFLAGS = -lpthread 

all: server client

server: server.o kosaraju_vector_list.o reactor.o
	$(CXX) $(CXXFLAGS) -o server server.o kosaraju_vector_list.o reactor.o $(LDFLAGS)

client: client.o
	$(CXX) $(CXXFLAGS) -o client client.o

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp

reactor.o: ../ex8/reactor.cpp
	$(CXX) $(CXXFLAGS) -c ../ex8/reactor.cpp -o reactor.o

kosaraju_vector_list.o: kosaraju_vector_list.cpp
	$(CXX) $(CXXFLAGS) -c kosaraju_vector_list.cpp -o kosaraju_vector_list.o

clean:
	rm -f server client server.o client.o reactor.o kosaraju_vector_list.o
