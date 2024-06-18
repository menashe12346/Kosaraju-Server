#ifndef KOSARAJU_VECTOR_LIST_H
#define KOSARAJU_VECTOR_LIST_H

#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

/// @brief Class to represent a directed graph and find its strongly connected components using Kosaraju's algorithm.
class KosarajuVectorList {
public:
    /// @brief Constructor to initialize the graph with given vertices and edges.
    /// @param n Number of vertices in the graph.
    /// @param edges Vector of edges where each edge is represented as a pair of integers.
    KosarajuVectorList(int n, const vector<pair<int, int>>& edges);

    /// @brief Function to find all strongly connected components (SCCs) of the graph.
    void findSCCs();

    /// @brief Function to print the strongly connected components (SCCs).
    void printSCCs() const;

    /// @brief Function to print the graph.
    void printGraph() const;

    /// @brief Function to add an edge to the graph.
    /// @param u The start vertex of the edge.
    /// @param v The end vertex of the edge.
    void addEdge(int u, int v);

    /// @brief Function to remove an edge from the graph.
    /// @param u The start vertex of the edge.
    /// @param v The end vertex of the edge.
    void removeEdge(int u, int v);

    /// @brief Function to get the number of vertices in the graph.
    /// @return The number of vertices in the graph.
    int getNumVertices() const { return n; }

    /// @brief Function to get the strongly connected components (SCCs) of the graph.
    /// @return A vector of SCCs, where each SCC is represented as a vector of integers.
    vector<vector<int>> getSCCs() const { return sccs; }

    /// @brief Function to get the size of the largest SCC.
    /// @return The size of the largest SCC.
    int largestSCCSize() const;

private:
    int n; ///< Number of vertices in the graph.
    vector<list<int>> graph; ///< Adjacency list representation of the graph.
    vector<list<int>> transposedGraph; ///< Adjacency list of the transposed graph.
    vector<bool> visited; ///< Vector to keep track of visited vertices.
    stack<int> finishStack; ///< Stack to store the vertices in the order of their finishing times.
    vector<vector<int>> sccs; ///< Vector to store the strongly connected components (SCCs).

    /// @brief Depth-first search (DFS) function for the first pass of Kosaraju's algorithm.
    /// @param node The current node to visit.
    void dfsFirstPass(int node);

    /// @brief Depth-first search (DFS) function for the second pass of Kosaraju's algorithm.
    /// @param node The current node to visit.
    /// @param scc The current strongly connected component (SCC) being formed.
    void dfsSecondPass(int node, vector<int>& scc);
};

#endif // KOSARAJU_VECTOR_LIST_H
