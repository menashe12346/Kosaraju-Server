#ifndef KOSARAJU_LINKED_LIST_HPP
#define KOSARAJU_LINKED_LIST_HPP

#include <iostream>
#include <stack>
#include <vector>
#include <list>

using namespace std;

// Node class for the linked list
class Node {
public:
    int data;
    Node* next;
    Node(int data);
};

// Linked list class to manage the list operations
class LinkedList {
public:
    Node* head;
    LinkedList();
    void push(int data);
    void clear();
    void print() const;
};

// KosarajuList Implementation
class KosarajuLinkedList {
public:
    KosarajuLinkedList(int n, const vector<pair<int, int>>& edges);
    void findSCCs();
    void printSCCs() const;

private:
    int n;
    vector<Node*> graph;
    vector<Node*> transposedGraph;
    vector<bool> visited;
    stack<int> finishStack;
    vector<LinkedList> sccs;

    void addEdge(Node*& head, int data);
    void dfsFirstPass(int node);
    void dfsSecondPass(int node, LinkedList& scc);
};

#endif // KOSARAJU_LIST_HPP
