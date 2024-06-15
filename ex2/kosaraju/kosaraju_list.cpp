#include "kosaraju_list.hpp"

KosarajuList::KosarajuList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);
    transposedGraph.resize(n);
    visited.resize(n, false);
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            auto itGraph = next(graph.begin(), edge.first - 1);  // Convert to zero-based index
            auto itTransposedGraph = next(transposedGraph.begin(), edge.second - 1);  // Convert to zero-based index
            itGraph->push_back(edge.second - 1);  // Convert to zero-based index
            itTransposedGraph->push_back(edge.first - 1);  // Convert to zero-based index
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;
        }
    }
}

void KosarajuList::findSCCs() {
    for (int i = 0; i < n; ++i) {
        auto itVisited = next(visited.begin(), i);
        if (!(*itVisited)) {
            dfsFirstPass(i);
        }
    }

    fill(visited.begin(), visited.end(), false);
    while (!finishStack.empty()) {
        int node = finishStack.top();
        finishStack.pop();
        auto itVisited = next(visited.begin(), node);
        if (!(*itVisited)) {
            list<int> scc;
            dfsSecondPass(node, scc);
            sccs.push_back(scc);
        }
    }
}

void KosarajuList::printSCCs() const {
    cout << "\nKosaraju List algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node << " ";
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuList::dfsFirstPass(int node) {
    auto itVisited = next(visited.begin(), node);
    *itVisited = true;
    auto itGraph = next(graph.begin(), node);
    for (int neighbor : *itGraph) {
        auto itNeighborVisited = next(visited.begin(), neighbor);
        if (!(*itNeighborVisited)) {
            dfsFirstPass(neighbor);
        }
    }
    finishStack.push(node);
}

void KosarajuList::dfsSecondPass(int node, list<int>& scc) {
    auto itVisited = next(visited.begin(), node);
    *itVisited = true;
    scc.push_back(node);
    auto itTransposedGraph = next(transposedGraph.begin(), node);
    for (int neighbor : *itTransposedGraph) {
        auto itNeighborVisited = next(visited.begin(), neighbor);
        if (!(*itNeighborVisited)) {
            dfsSecondPass(neighbor, scc);
        }
    }
}
