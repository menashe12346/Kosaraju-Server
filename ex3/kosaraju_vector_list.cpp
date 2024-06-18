#include "kosaraju_vector_list.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

KosarajuVectorList::KosarajuVectorList(int n, const vector<pair<int, int>>& edges) : n(n) {
    // Initialize the graph with n+1 nodes to accommodate 1-based indexing
    graph.resize(n + 1);
    // Initialize the transposed graph with n+1 nodes to accommodate 1-based indexing
    transposedGraph.resize(n + 1);
    // Initialize the visited vector with n+1 nodes to accommodate 1-based indexing
    visited.resize(n + 1, false);
    for (const auto& edge : edges) {
        // Add edge to the graph (convert to one-based index)
        graph[edge.first].push_back(edge.second);
        // Add edge to the transposed graph (convert to one-based index)
        transposedGraph[edge.second].push_back(edge.first);
    }
}

void KosarajuVectorList::findSCCs() {
    sccs.clear(); // Clear the SCCs vector before finding SCCs
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        finishStack.pop(); // Clear the finish stack
    }

    // First Pass
    for (int i = 1; i <= n; ++i) {
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
            cout << node << " ";  // Output the node (already adjusted for 1-based index)
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuVectorList::printGraph() const {
    cout << "\nCurrent Graph (Adjacency List):\n";
    for (int i = 1; i <= n; ++i) {
        cout << i << " -> ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

void KosarajuVectorList::dfsFirstPass(int node) {
    visited[node] = true; // Mark the node as visited
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor); // Recursively visit all neighbors
        }
    }
    finishStack.push(node); // Push the node to the finish stack after all neighbors are visited
}

void KosarajuVectorList::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true; // Mark the node as visited
    scc.push_back(node); // Add the node to the current SCC
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc); // Recursively visit all neighbors
        }
    }
}

void KosarajuVectorList::addEdge(int u, int v) {
    graph[u].push_back(v); // Add edge to the graph
    transposedGraph[v].push_back(u); // Add edge to the transposed graph
}

void KosarajuVectorList::removeEdge(int u, int v) {
    graph[u].erase(remove(graph[u].begin(), graph[u].end(), v), graph[u].end()); // Remove edge from the graph
    transposedGraph[v].erase(remove(transposedGraph[v].begin(), transposedGraph[v].end(), u), transposedGraph[v].end()); // Remove edge from the transposed graph
}
