#include "kosaraju_vector_list.hpp"

KosarajuList::KosarajuList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);
    transposedGraph.resize(n);
    visited.resize(n, false);
    for (const auto& edge : edges) {
        graph[edge.first - 1].push_back(edge.second - 1);
        transposedGraph[edge.second - 1].push_back(edge.first - 1);
    }
}

void KosarajuList::findSCCs() {
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

void KosarajuList::printSCCs() const {
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

void KosarajuList::dfsFirstPass(int node) {
    visited[node] = true;
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor);
        }
    }
    finishStack.push(node);
}

void KosarajuList::dfsSecondPass(int node, vector<int>& scc) {
    visited[node] = true;
    scc.push_back(node);
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc);
        }
    }
}
