#include "kosaraju_list.hpp"

KosarajuList::KosarajuList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n + 1);  // Resize the graph to hold n+1 nodes (1-based index)
    transposedGraph.resize(n + 1);  // Resize the transposed graph
    visited.resize(n + 1, false);  // Initialize visited vector with false
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            auto itGraph = next(graph.begin(), edge.first);  // Use 1-based index
            auto itTransposedGraph = next(transposedGraph.begin(), edge.second);  // Use 1-based index
            itGraph->push_back(edge.second);  // Add edge to the graph (1-based index)
            itTransposedGraph->push_back(edge.first);  // Add edge to the transposed graph (1-based index)
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;  // Print error for invalid edges
        }
    }
}

const std::list<std::list<int>>& KosarajuList::getSCCs() const {
    return sccs;
}

void KosarajuList::findSCCs() {
    sccs.clear(); // Clear the SCCs vector before finding SCCs
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        finishStack.pop(); // Clear the finish stack
    }
    for (int i = 1; i <= n; ++i) {  // Start loop from 1 instead of 0
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
