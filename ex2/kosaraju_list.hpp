#ifndef KOSARAJU_LIST_H
#define KOSARAJU_LIST_H

#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

class KosarajuList {
public:
    KosarajuList(int n, const vector<pair<int, int>>& edges);
    void findSCCs();
    void printSCCs() const;

private:
    int n;
    list<list<int>> graph;
    list<list<int>> transposedGraph;
    list<bool> visited;
    stack<int> finishStack;
    list<list<int>> sccs;

    void dfsFirstPass(int node);
    void dfsSecondPass(int node, list<int>& scc);
};

#endif // KOSARAJU_LIST_H
