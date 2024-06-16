#ifndef KOSARAJU_LINKED_LIST_HPP
#define KOSARAJU_LINKED_LIST_HPP

#include <iostream>
#include <stack>
#include <vector>
#include <list>

using namespace std;

/// @brief Node class for the linked list
class Node {
public:
    int data;
    Node* next;

    /// @brief Constructor to initialize a Node with data
    /// @param data The data to be stored in the Node
    Node(int data);
};

/// @brief Linked list class to manage the list operations
class LinkedList {
public:
    Node* head;

    /// @brief Constructor to initialize an empty LinkedList
    LinkedList();

    /// @brief Pushes a new data value onto the front of the list
    /// @param data The data to be added to the list
    void push(int data);

    /// @brief Clears the linked list, deleting all nodes
    void clear();

    /// @brief Prints the contents of the linked list
    void print() const;
};

/// @brief KosarajuLinkedList class to implement Kosaraju's algorithm for finding SCCs
class KosarajuLinkedList {
public:
    /// @brief Constructor to initialize the graph and transposed graph
    /// @param n The number of nodes in the graph
    /// @param edges The edges of the graph
    KosarajuLinkedList(int n, const vector<pair<int, int>>& edges);

    /// @brief Finds and stores the strongly connected components (SCCs) of the graph
    void findSCCs();

    /// @brief Prints the strongly connected components (SCCs)
    void printSCCs() const;

private:
    int n;  // Number of nodes
    vector<Node*> graph;  // Adjacency list of the graph
    vector<Node*> transposedGraph;  // Adjacency list of the transposed graph
    vector<bool> visited;  // Visited flag for nodes
    stack<int> finishStack;  // Stack to store finish times of nodes
    vector<LinkedList> sccs;  // List of strongly connected components

    /// @brief Adds an edge to the linked list representation of a graph
    /// @param head The head of the linked list
    /// @param data The data (node value) to be added
    void addEdge(Node*& head, int data);

    /// @brief Depth-first search for the first pass (filling finish stack)
    /// @param node The starting node for DFS
    void dfsFirstPass(int node);

    /// @brief Depth-first search for the second pass (collecting SCCs)
    /// @param node The starting node for DFS
    /// @param scc The linked list to store the current SCC
    void dfsSecondPass(int node, LinkedList& scc);
};

#endif // KOSARAJU_LIST_HPP
