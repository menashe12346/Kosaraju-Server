#include "kosaraju_deque.hpp"

KosarajuDeque::KosarajuDeque(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n + 1);  // Initialize the graph with n+1 nodes (1-based index)
    transposedGraph.resize(n + 1);  // Initialize the transposed graph with n+1 nodes (1-based index)
    visited.resize(n + 1, false);  // Initialize the visited vector with false
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            graph[edge.first].push_back(edge.second);  // Add edge to the graph (1-based index)
            transposedGraph[edge.second].push_back(edge.first);  // Add edge to the transposed graph (1-based index)
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;  // Print error for invalid edges
        }
    }
}

const std::deque<std::deque<int>>& KosarajuDeque::getSCCs() const {
    return sccs;
}

void KosarajuDeque::findSCCs() {
    sccs.clear(); // Clear the SCCs vector before finding SCCs
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        finishStack.pop(); // Clear the finish stack
    }
    for (int i = 1; i <= n; ++i) {  // Start loop from 1 instead of 0
        if (!visited[i]) {
            dfsFirstPass(i);  // Perform DFS for the first pass
        }
    }

    fill(visited.begin(), visited.end(), false);  // Reset the visited vector for the second pass
    while (!finishStack.empty()) {
        int node = finishStack.top();  // Get the top node from the finish stack
        finishStack.pop();  // Remove the node from the stack
        if (!visited[node]) {
            deque<int> scc;  // Create a new deque to store the SCC
            dfsSecondPass(node, scc);  // Perform DFS for the second pass
            sccs.push_back(scc);  // Add the SCC to the list of SCCs
        }
    }
}

void KosarajuDeque::printSCCs() const {
    cout << "\nKosaraju Deque algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node << " ";  // Print each node in the SCC (1-based index)
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuDeque::dfsFirstPass(int node) {
    visited[node] = true;  // Mark the node as visited
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor);  // Recursively visit all unvisited neighbors
        }
    }
    finishStack.push(node);  // Push the node onto the finish stack after visiting all its neighbors
}

void KosarajuDeque::dfsSecondPass(int node, deque<int>& scc) {
    visited[node] = true;  // Mark the node as visited
    scc.push_back(node);  // Add the node to the current SCC
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc);  // Recursively visit all unvisited neighbors
        }
    }
}
