#include "kosaraju_vector_list.hpp"

KosarajuVectorList::KosarajuVectorList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);  // Initialize the graph with n nodes
    transposedGraph.resize(n);  // Initialize the transposed graph with n nodes
    visited.resize(n, false);  // Initialize the visited vector with false
    for (const auto& edge : edges) {
        graph[edge.first - 1].push_back(edge.second - 1);  // Add edge to the graph (convert to zero-based index)
        transposedGraph[edge.second - 1].push_back(edge.first - 1);  // Add edge to the transposed graph (convert to zero-based index)
    }
}

void KosarajuVectorList::findSCCs() {
    // First Pass
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfsFirstPass(i);  // Perform DFS to fill the finish stack
        }
    }

    // Second Pass
    fill(visited.begin(), visited.end(), false);  // Reset the visited vector for the second pass
    while (!finishStack.empty()) {
        int node = finishStack.top();  // Get the top node from the finish stack
        finishStack.pop();  // Remove the top node from the stack
        if (!visited[node]) {
            vector<int> scc;  // Create a new vector to store the SCC
            dfsSecondPass(node, scc);  // Perform DFS to collect nodes in the SCC
            sccs.push_back(scc);  // Add the SCC to the list of SCCs
        }
    }
}

void KosarajuVectorList::printSCCs() const {
    cout << "\nKosaraju Vector List algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node + 1 << " ";  // Convert back to 1-based index for output
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuVectorList::dfsFirstPass(int node) {
    visited[node] = true;  // Mark the node as visited
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor);  // Recursively visit all unvisited adjacent nodes
        }
    }
    finishStack.push(node);  // Push the node onto the finish stack after visiting all its neighbors
}

void KosarajuVectorList::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true;  // Mark the node as visited
    scc.push_back(node);  // Add the node to the current SCC
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc);  // Recursively visit all unvisited adjacent nodes
        }
    }
}
