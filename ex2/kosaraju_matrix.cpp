#include "kosaraju_matrix.hpp"

KosarajuMatrix::KosarajuMatrix(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n, vector<bool>(n, false));
    transposedGraph.resize(n, vector<bool>(n, false));
    visited.resize(n, false);
    for (const auto& edge : edges) {
        graph[edge.first - 1][edge.second - 1] = true;
        transposedGraph[edge.second - 1][edge.first - 1] = true;
    }
}

void KosarajuMatrix::findSCCs() {
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

void KosarajuMatrix::printSCCs() const {
    cout << "Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node + 1 << " ";  // Convert back to 1-based index for output
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuMatrix::dfsFirstPass(int node) {
    visited[node] = true;
    for (int i = 0; i < n; ++i) {
        if (graph[node][i] && !visited[i]) {
            dfsFirstPass(i);
        }
    }
    finishStack.push(node);
}

void KosarajuMatrix::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true;
    scc.push_back(node);
    for (int i = 0; i < n; ++i) {
        if (transposedGraph[node][i] && !visited[i]) {
            dfsSecondPass(i, scc);
        }
    }
}
