#ifndef KOSARAJU_DEQUE_H
#define KOSARAJU_DEQUE_H

#include <iostream>
#include <deque>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

/// @brief KosarajuDeque class to implement Kosaraju's algorithm for finding SCCs using deque
class KosarajuDeque {
public:
    /// @brief Constructor to initialize the graph and transposed graph
    /// @param n The number of nodes in the graph
    /// @param edges The edges of the graph
    KosarajuDeque(int n, const vector<pair<int, int>>& edges);

    const std::deque<std::deque<int>>& getSCCs() const;

    /// @brief Finds and stores the strongly connected components (SCCs) of the graph
    void findSCCs();

    /// @brief Prints the strongly connected components (SCCs)
    void printSCCs() const;

private:
    int n;  ///< Number of nodes
    deque<deque<int>> graph;  ///< Adjacency list of the graph
    deque<deque<int>> transposedGraph;  ///< Adjacency list of the transposed graph
    deque<bool> visited;  ///< Visited flag for nodes
    stack<int> finishStack;  ///< Stack to store finish times of nodes
    deque<deque<int>> sccs;  ///< List of strongly connected components

    /// @brief Depth-first search for the first pass (filling finish stack)
    /// @param node The starting node for DFS
    void dfsFirstPass(int node);

    /// @brief Depth-first search for the second pass (collecting SCCs)
    /// @param node The starting node for DFS
    /// @param scc The deque to store the current SCC
    void dfsSecondPass(int node, deque<int>& scc);
};

#endif // KOSARAJU_DEQUE_H
