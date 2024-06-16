#include "kosaraju_vector_list.hpp"

KosarajuVectorList::KosarajuVectorList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);
    transposedGraph.resize(n);
    visited.resize(n, false);
    for (const auto& edge : edges) {
        graph[edge.first - 1].push_back(edge.second - 1);
        transposedGraph[edge.second - 1].push_back(edge.first - 1);
    }
}

void KosarajuVectorList::findSCCs() {
    // First Pass
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfsFirstPass(i);
        }
    }

    // Second Pass
    fill(visited.begin(), visited.end(), false);
    while (!finishStack.empty()) {
        int node = finishStack.top();
        finishStack.pop();
        if (!visited[node]) {
            vector<int> scc;
            dfsSecondPass(node, scc);
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
            cout << (hasEdge ? "1" : "0") << " ";
        }
        cout << "\n";
    }

    // Print edges list
    cout << "\nEdges:\n";
    for (int i = 0; i < n; ++i) {
        for (int neighbor : graph[i]) {
            cout << (i + 1) << " -> " << (neighbor + 1) << "\n";
        }
    }
}

void KosarajuVectorList::dfsFirstPass(int node) {
    visited[node] = true;
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor);
        }
    }
    finishStack.push(node);
}

void KosarajuVectorList::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true;
    scc.push_back(node);
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc);
        }
    }
}

void KosarajuVectorList::addEdge(int u, int v) {
    graph[u - 1].push_back(v - 1);
    transposedGraph[v - 1].push_back(u - 1);
}

void KosarajuVectorList::removeEdge(int u, int v) {
    graph[u - 1].remove(v - 1);
    transposedGraph[v - 1].remove(u - 1);
}