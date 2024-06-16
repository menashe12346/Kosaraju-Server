#ifndef KOSARAJU_VECTOR_LIST_H
#define KOSARAJU_VECTOR_LIST_H

#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

class KosarajuVectorList {
public:
    KosarajuVectorList(int n, const vector<pair<int, int>>& edges);
    void findSCCs();
    void printSCCs() const;

    void addEdge(int u, int v);
    void removeEdge(int u, int v);

    void printGraph() const; // for ex4, the users can create new graphs so we will know whats the current graph.

private:
    int n;
    vector<list<int>> graph;
    vector<list<int>> transposedGraph;
    vector<bool> visited;
    stack<int> finishStack;
    vector<vector<int>> sccs;

    void dfsFirstPass(int node);
    void dfsSecondPass(int node, vector<int>& scc);
};

#endif // KOSARAJU_VECTOR_LIST_H
