#ifndef KOSARAJU_LIST_H
#define KOSARAJU_LIST_H

#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

/// @brief KosarajuList class to implement Kosaraju's algorithm for finding SCCs using list
class KosarajuList {
public:
    /// @brief Constructor to initialize the graph and transposed graph
    /// @param n The number of nodes in the graph
    /// @param edges The edges of the graph
    KosarajuList(int n, const vector<pair<int, int>>& edges);

    /// @brief Finds and stores the strongly connected components (SCCs) of the graph
    void findSCCs();

    /// @brief Prints the strongly connected components (SCCs)
    void printSCCs() const;

private:
    int n;  ///< Number of nodes
    list<list<int>> graph;  ///< Adjacency list of the graph
    list<list<int>> transposedGraph;  ///< Adjacency list of the transposed graph
    list<bool> visited;  ///< Visited flag for nodes
    stack<int> finishStack;  ///< Stack to store finish times of nodes
    list<list<int>> sccs;  ///< List of strongly connected components

    /// @brief Depth-first search for the first pass (filling finish stack)
    /// @param node The starting node for DFS
    void dfsFirstPass(int node);

    /// @brief Depth-first search for the second pass (collecting SCCs)
    /// @param node The starting node for DFS
    /// @param scc The list to store the current SCC
    void dfsSecondPass(int node, list<int>& scc);
};

#endif // KOSARAJU_LIST_H
