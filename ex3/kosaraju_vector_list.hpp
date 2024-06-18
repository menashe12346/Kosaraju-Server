#ifndef KOSARAJU_VECTOR_LIST_H
#define KOSARAJU_VECTOR_LIST_H

#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

/// @brief Class implementing Kosaraju's algorithm using a vector of lists representation of the graph.
class KosarajuVectorList {
public:
    /// @brief Constructor to initialize the graph and transposed graph.
    /// @param n The number of nodes in the graph.
    /// @param edges The edges of the graph.
    KosarajuVectorList(int n, const vector<pair<int, int>>& edges);

    /// @brief Finds and stores all Strongly Connected Components (SCCs) in the graph.
    void findSCCs();

    /// @brief Prints all the SCCs found in the graph.
    void printSCCs() const;

    /// @brief Adds an edge to the graph.
    /// @param u The starting node of the edge.
    /// @param v The ending node of the edge.
    void addEdge(int u, int v);

    /// @brief Removes an edge from the graph.
    /// @param u The starting node of the edge.
    /// @param v The ending node of the edge.
    void removeEdge(int u, int v);

    /// @brief Prints the current state of the graph.
    void printGraph() const; // for ex4, the users can create new graphs so we will know whats the current graph.

private:
    int n;  ///< Number of nodes in the graph.
    vector<list<int>> graph;  ///< Adjacency list of the graph.
    vector<list<int>> transposedGraph;  ///< Adjacency list of the transposed graph.
    vector<bool> visited;  ///< Visited nodes for DFS.
    stack<int> finishStack;  ///< Stack to store the finish times of nodes in the first pass of DFS.
    vector<vector<int>> sccs;  ///< Vector to store the SCCs.

    /// @brief Depth-First Search (DFS) for the first pass to fill the finish stack.
    /// @param node The starting node for DFS.
    void dfsFirstPass(int node);

    /// @brief Depth-First Search (DFS) for the second pass to discover SCCs.
    /// @param node The starting node for DFS.
    /// @param scc The current SCC being discovered.
    void dfsSecondPass(int node, vector<int>& scc);
};

#endif // KOSARAJU_VECTOR_LIST_H
