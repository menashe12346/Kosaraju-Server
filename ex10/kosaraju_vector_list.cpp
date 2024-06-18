#include "kosaraju_vector_list.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

KosarajuVectorList::KosarajuVectorList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);
    transposedGraph.resize(n);
    visited.resize(n, false);
    for (const auto& edge : edges) {
        graph[edge.first - 1].push_back(edge.second - 1); // Adjust for 0-based indexing
        transposedGraph[edge.second - 1].push_back(edge.first - 1); // Create transposed graph
    }
}

void KosarajuVectorList::findSCCs() {
    sccs.clear(); // Clear the SCCs vector before finding SCCs
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        finishStack.pop(); // Clear the finish stack
    }

    // First Pass
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfsFirstPass(i); // Perform DFS to fill the finish stack
        }
    }

    // Second Pass
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        int node = finishStack.top();
        finishStack.pop();
        if (!visited[node]) {
            vector<int> scc;
            dfsSecondPass(node, scc); // Perform DFS to find SCCs
            sccs.push_back(scc);
        }
    }
}

void KosarajuVectorList::printSCCs() const {
    cout << "\nKosaraju Vector List algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node + 1 << " "; // Convert back to 1-based index for output
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuVectorList::printGraph() const {
    cout << "\nCurrent Graph (Adjacency Matrix):" << endl;
    cout << "    ";
    for (int i = 0; i < n; ++i) {
        cout << i + 1 << " ";
    }
    cout << "\n   " << string(n * 2, '-') << endl;
    for (int i = 0; i < n; ++i) {
        cout << i + 1 << " | ";
        for (int j = 0; j < n; ++j) {
            if (find(graph[i].begin(), graph[i].end(), j) != graph[i].end()) {
                cout << "1 "; // Print 1 if there is an edge
            } else {
                cout << "0 "; // Print 0 if there is no edge
            }
        }
        cout << endl;
    }

    cout << "\nEdges:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int neighbor : graph[i]) {
            cout << i + 1 << " -> " << neighbor + 1 << endl; // Print all edges
        }
    }
}

void KosarajuVectorList::addEdge(int u, int v) {
    graph[u - 1].push_back(v - 1); // Add edge to the graph
    transposedGraph[v - 1].push_back(u - 1); // Add edge to the transposed graph
}

void KosarajuVectorList::removeEdge(int u, int v) {
    graph[u - 1].erase(remove(graph[u - 1].begin(), graph[u - 1].end(), v - 1), graph[u - 1].end()); // Remove edge from the graph
    transposedGraph[v - 1].erase(remove(transposedGraph[v - 1].begin(), transposedGraph[v - 1].end(), u - 1), transposedGraph[v - 1].end()); // Remove edge from the transposed graph
}

void KosarajuVectorList::dfsFirstPass(int node) {
    visited[node] = true; // Mark the node as visited
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor); // Recursively visit all neighbors
        }
    }
    finishStack.push(node); // Push the node to the finish stack after visiting all neighbors
}

void KosarajuVectorList::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true; // Mark the node as visited
    scc.push_back(node); // Add the node to the current SCC
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc); // Recursively visit all neighbors in the transposed graph
        }
    }
}

int KosarajuVectorList::largestSCCSize() const {
    int maxSize = 0;
    for (const auto& scc : sccs) {
        if (scc.size() > maxSize) {
            maxSize = scc.size();
        }
    }
    return maxSize;
}
