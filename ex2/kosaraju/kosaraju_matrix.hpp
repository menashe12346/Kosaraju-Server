#ifndef KOSARAJU_MATRIX_H
#define KOSARAJU_MATRIX_H

#include <iostream>
#include <vector>
#include <stack>
#include <chrono>

using namespace std;

/// @brief KosarajuMatrix class to implement Kosaraju's algorithm for finding SCCs using adjacency matrix
class KosarajuMatrix {
public:
    /// @brief Constructor to initialize the graph and transposed graph
    /// @param n The number of nodes in the graph
    /// @param edges The edges of the graph
    KosarajuMatrix(int n, const vector<pair<int, int>>& edges);

    const std::vector<std::vector<int>>& getSCCs() const;

    /// @brief Finds and stores the strongly connected components (SCCs) of the graph
    void findSCCs();

    /// @brief Prints the strongly connected components (SCCs)
    void printSCCs() const;

private:
    int n;  ///< Number of nodes
    vector<vector<bool>> graph;  ///< Adjacency matrix of the graph
    vector<vector<bool>> transposedGraph;  ///< Adjacency matrix of the transposed graph
    vector<bool> visited;  ///< Visited flag for nodes
    stack<int> finishStack;  ///< Stack to store finish times of nodes
    vector<vector<int>> sccs;  ///< List of strongly connected components

    /// @brief Depth-first search for the first pass (filling finish stack)
    /// @param node The starting node for DFS
    void dfsFirstPass(int node);

    /// @brief Depth-first search for the second pass (collecting SCCs)
    /// @param node The starting node for DFS
    /// @param scc The vector to store the current SCC
    void dfsSecondPass(int node, vector<int>& scc);
};

#endif // KOSARAJU_MATRIX_H
