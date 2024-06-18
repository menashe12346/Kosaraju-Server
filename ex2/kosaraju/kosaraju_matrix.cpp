#include "kosaraju_matrix.hpp"

KosarajuMatrix::KosarajuMatrix(int n, const vector<pair<int, int>>& edges) : n(n) {
    // Initialize the graph matrix with false
    graph.resize(n + 1, vector<bool>(n + 1, false));
    // Initialize the transposed graph matrix with false
    transposedGraph.resize(n + 1, vector<bool>(n + 1, false));
    // Initialize the visited vector with false
    visited.resize(n + 1, false);
    for (const auto& edge : edges) {
        // Add edge to the graph (convert to one-based index)
        graph[edge.first][edge.second] = true;
        // Add edge to the transposed graph (convert to one-based index)
        transposedGraph[edge.second][edge.first] = true;
    }
}

const std::vector<std::vector<int>>& KosarajuMatrix::getSCCs() const {
    return sccs;
}

void KosarajuMatrix::findSCCs() {
    sccs.clear(); // Clear the SCCs vector before finding SCCs
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        finishStack.pop(); // Clear the finish stack
    }
    // First Pass
    for (int i = 1; i <= n; ++i) {
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

void KosarajuMatrix::printSCCs() const {
    cout << "\nKosaraju Matrix algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node << " ";  // Output the node (already adjusted for 1-based index)
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuMatrix::dfsFirstPass(int node) {
    visited[node] = true;  // Mark the node as visited
    for (int i = 1; i <= n; ++i) {
        if (graph[node][i] && !visited[i]) {
            dfsFirstPass(i);  // Recursively visit all unvisited adjacent nodes
        }
    }
    finishStack.push(node);  // Push the node onto the finish stack after visiting all its neighbors
}

void KosarajuMatrix::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true;  // Mark the node as visited
    scc.push_back(node);  // Add the node to the current SCC
    for (int i = 1; i <= n; ++i) {
        if (transposedGraph[node][i] && !visited[i]) {
            dfsSecondPass(i, scc);  // Recursively visit all unvisited adjacent nodes
        }
    }
}
