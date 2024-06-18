#include "kosaraju_vector_list.hpp"

KosarajuVectorList::KosarajuVectorList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);
    transposedGraph.resize(n);
    visited.resize(n, false);
    for (const auto& edge : edges) {
        graph[edge.first - 1].push_back(edge.second - 1); // Add edge to the graph
        transposedGraph[edge.second - 1].push_back(edge.first - 1); // Add edge to the transposed graph
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
            cout << node + 1 << " ";  // Convert back to 1-based index for output
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuVectorList::printGraph() const {
    cout << "\nCurrent Graph (Adjacency Matrix):\n";
    cout << "    ";
    for (int i = 0; i < n; ++i) {
        cout << (i + 1) << " ";
    }
    cout << "\n   ";
    for (int i = 0; i < n; ++i) {
        cout << "---";
    }
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << (i + 1) << " | ";
        for (int j = 0; j < n; ++j) {
            bool hasEdge = false;
            for (int neighbor : graph[i]) {
                if (neighbor == j) {
                    hasEdge = true;
                    break;
                }
            }
            cout << (hasEdge ? "1" : "0") << " "; // Print 1 if there is an edge, otherwise 0
        }
        cout << "\n";
    }

    // Print edges list
    cout << "\nEdges:\n";
    for (int i = 0; i < n; ++i) {
        for (int neighbor : graph[i]) {
            cout << (i + 1) << " -> " << (neighbor + 1) << "\n"; // Print the edges
        }
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
    graph[u - 1].push_back(v - 1); // Add edge to the graph
    transposedGraph[v - 1].push_back(u - 1); // Add edge to the transposed graph
}

void KosarajuVectorList::removeEdge(int u, int v) {
    graph[u - 1].remove(v - 1); // Remove edge from the graph
    transposedGraph[v - 1].remove(u - 1); // Remove edge from the transposed graph
}
