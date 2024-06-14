#include "kosaraju_deque.hpp"

KosarajuDeque::KosarajuDeque(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);
    transposedGraph.resize(n);
    visited.resize(n, false);
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            graph[edge.first - 1].push_back(edge.second - 1);  // Convert to zero-based index
            transposedGraph[edge.second - 1].push_back(edge.first - 1);  // Convert to zero-based index
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;
        }
    }
}

void KosarajuDeque::findSCCs() {
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfsFirstPass(i);
        }
    }

    fill(visited.begin(), visited.end(), false);
    while (!finishStack.empty()) {
        int node = finishStack.top();
        finishStack.pop();
        if (!visited[node]) {
            deque<int> scc;
            dfsSecondPass(node, scc);
            sccs.push_back(scc);
        }
    }
}

void KosarajuDeque::printSCCs() const {
    cout << "Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node << " ";
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuDeque::dfsFirstPass(int node) {
    visited[node] = true;
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFirstPass(neighbor);
        }
    }
    finishStack.push(node);
}

void KosarajuDeque::dfsSecondPass(int node, deque<int>& scc) {
    visited[node] = true;
    scc.push_back(node);
    for (int neighbor : transposedGraph[node]) {
        if (!visited[neighbor]) {
            dfsSecondPass(neighbor, scc);
        }
    }
}
