#ifndef KOSARAJU_MATRIX_H
#define KOSARAJU_MATRIX_H

#include <iostream>
#include <vector>
#include <stack>
#include <chrono>

using namespace std;

class KosarajuMatrix {
public:
    KosarajuMatrix(int n, const vector<pair<int, int>>& edges);
    void findSCCs();
    void printSCCs() const;

private:
    int n;
    vector<vector<bool>> graph;
    vector<vector<bool>> transposedGraph;
    vector<bool> visited;
    stack<int> finishStack;
    vector<vector<int>> sccs;

    void dfsFirstPass(int node);
    void dfsSecondPass(int node, vector<int>& scc);
};

#endif // KOSARAJU_MATRIX_H
