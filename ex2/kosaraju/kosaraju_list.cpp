#include "kosaraju_list.hpp"

KosarajuList::KosarajuList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n);  // Resize the graph to hold n nodes
    transposedGraph.resize(n);  // Resize the transposed graph
    visited.resize(n, false);  // Initialize visited vector with false
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            auto itGraph = next(graph.begin(), edge.first - 1);  // Convert to zero-based index
            auto itTransposedGraph = next(transposedGraph.begin(), edge.second - 1);  // Convert to zero-based index
            itGraph->push_back(edge.second - 1);  // Add edge to the graph (zero-based index)
            itTransposedGraph->push_back(edge.first - 1);  // Add edge to the transposed graph (zero-based index)
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;  // Print error for invalid edges
        }
    }
}

void KosarajuList::findSCCs() {
    for (int i = 0; i < n; ++i) {
        auto itVisited = next(visited.begin(), i);
        if (!(*itVisited)) {
            dfsFirstPass(i);  // Perform DFS to fill the finish stack
        }
    }

    fill(visited.begin(), visited.end(), false);  // Reset the visited vector for the second pass
    while (!finishStack.empty()) {
        int node = finishStack.top();  // Get the top node from the finish stack
        finishStack.pop();  // Remove the top node from the stack
        auto itVisited = next(visited.begin(), node);
        if (!(*itVisited)) {
            list<int> scc;  // Create a new list to store the SCC
            dfsSecondPass(node, scc);  // Perform DFS to collect nodes in the SCC
            sccs.push_back(scc);  // Add the SCC to the list of SCCs
        }
    }
}

void KosarajuList::printSCCs() const {
    cout << "\nKosaraju List algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        for (int node : scc) {
            cout << node << " ";  // Print each node in the SCC
        }
        cout << endl << "----------------" << endl;
    }
}

void KosarajuList::dfsFirstPass(int node) {
    auto itVisited = next(visited.begin(), node);
    *itVisited = true;  // Mark the node as visited
    auto itGraph = next(graph.begin(), node);
    for (int neighbor : *itGraph) {
        auto itNeighborVisited = next(visited.begin(), neighbor);
        if (!(*itNeighborVisited)) {
            dfsFirstPass(neighbor);  // Recursively visit all unvisited neighbors
        }
    }
    finishStack.push(node);  // Push the node onto the finish stack after visiting all its neighbors
}

void KosarajuList::dfsSecondPass(int node, list<int>& scc) {
    auto itVisited = next(visited.begin(), node);
    *itVisited = true;  // Mark the node as visited
    scc.push_back(node);  // Add the node to the current SCC
    auto itTransposedGraph = next(transposedGraph.begin(), node);
    for (int neighbor : *itTransposedGraph) {
        auto itNeighborVisited = next(visited.begin(), neighbor);
        if (!(*itNeighborVisited)) {
            dfsSecondPass(neighbor, scc);  // Recursively visit all unvisited neighbors
        }
    }
}
