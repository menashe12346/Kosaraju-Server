#ifndef KOSARAJU_DEQUE_H
#define KOSARAJU_DEQUE_H

#include <iostream>
#include <deque>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

class KosarajuDeque {
public:
    KosarajuDeque(int n, const vector<pair<int, int>>& edges);
    void findSCCs();
    void printSCCs() const;

private:
    int n;
    deque<deque<int>> graph;
    deque<deque<int>> transposedGraph;
    deque<bool> visited;
    stack<int> finishStack;
    deque<deque<int>> sccs;

    void dfsFirstPass(int node);
    void dfsSecondPass(int node, deque<int>& scc);
};

#endif // KOSARAJU_DEQUE_H
