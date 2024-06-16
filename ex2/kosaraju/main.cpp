#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include <map>
#include "kosaraju_deque.hpp"
#include "kosaraju_list.hpp"
#include "kosaraju_matrix.hpp"
#include "kosaraju_vector_list.hpp"
#include "../../ex1/kosaraju_linked_list.hpp"

using namespace std;
using namespace std::chrono; // for the time analysis

/// @brief Profiles the execution time of a given function.
/// @tparam Func A callable type (e.g., function, lambda)
/// @param name The name of the function being profiled
/// @param func The callable object to be profiled
/// @return The duration in seconds as a double
template <typename Func> // Allows writing generic functions that work with any type. The type is deduced by the compiler when the function is instantiated.
double profile_kosaraju(const std::string& name, Func func) {
    auto start = high_resolution_clock::now();  // Start timing
    func();  // Run the provided function
    auto end = high_resolution_clock::now();  // End timing
    duration<double> duration = end - start;  // Calculate the duration
    cout << "Time taken by " << name << ": " << duration.count() << " seconds\n";
    return duration.count();  // Return the duration
}

/// @brief Runs Kosaraju's algorithm using a linked list representation of the graph
/// @param n The number of nodes in the graph
/// @param edges The edges of the graph
void run_kosaraju_linked_list(int n, const vector<pair<int, int>>& edges) {
    KosarajuLinkedList kosarajuLinkedList(n, edges);  // Initialize the KosarajuLinkedList
    kosarajuLinkedList.findSCCs();  // Find SCCs
    kosarajuLinkedList.printSCCs();  // Print SCCs
}

/// @brief Runs Kosaraju's algorithm using a deque representation of the graph
/// @param n The number of nodes in the graph
/// @param edges The edges of the graph
void run_kosaraju_deque(int n, const vector<pair<int, int>>& edges) {
    KosarajuDeque kosarajuDeque(n, edges);  // Initialize the KosarajuDeque
    kosarajuDeque.findSCCs();  // Find SCCs
    kosarajuDeque.printSCCs();  // Print SCCs
}

/// @brief Runs Kosaraju's algorithm using a list representation of the graph
/// @param n The number of nodes in the graph
/// @param edges The edges of the graph
void run_kosaraju_list(int n, const vector<pair<int, int>>& edges) {
    KosarajuList kosarajuList(n, edges);  // Initialize the KosarajuList
    kosarajuList.findSCCs();  // Find SCCs
    kosarajuList.printSCCs();  // Print SCCs
}

/// @brief Runs Kosaraju's algorithm using a matrix representation of the graph
/// @param n The number of nodes in the graph
/// @param edges The edges of the graph
void run_kosaraju_matrix(int n, const vector<pair<int, int>>& edges) {
    KosarajuMatrix kosarajuMatrix(n, edges);  // Initialize the KosarajuMatrix
    kosarajuMatrix.findSCCs();  // Find SCCs
    kosarajuMatrix.printSCCs();  // Print SCCs
}

/// @brief Runs Kosaraju's algorithm using a vector of lists representation of the graph
/// @param n The number of nodes in the graph
/// @param edges The edges of the graph
void run_kosaraju_vector_list(int n, const vector<pair<int, int>>& edges) {
    KosarajuVectorList kosarajuVectorList(n, edges);  // Initialize the KosarajuVectorList
    kosarajuVectorList.findSCCs();  // Find SCCs
    kosarajuVectorList.printSCCs();  // Print SCCs
}

int main() {
    cout << "Reading number of vertices and edges..." << endl;

    // Read the number of vertices and edges
    int n, m;
    cin >> n >> m;
    cout << "Number of vertices: " << n << ", Number of edges: " << m << endl;
    vector<pair<int, int>> edges(m);

    cout << "Reading edges..." << endl;

    // Read the edges
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].first >> edges[i].second;
        cout << "Edge " << i << ": " << edges[i].first << " -> " << edges[i].second << endl;
    }

    map<string, double> timings;  // Map to store timings of each implementation
    // [&] - used to capture all variables from the surrounding scope by reference.
    timings["Kosaraju with Linked List"] = profile_kosaraju("Kosaraju with Linked List", [&]() { run_kosaraju_linked_list(n, edges); });
    timings["Kosaraju with Deque"] = profile_kosaraju("Kosaraju with Deque", [&]() { run_kosaraju_deque(n, edges); });
    timings["Kosaraju with List"] = profile_kosaraju("Kosaraju with List", [&]() { run_kosaraju_list(n, edges); });
    timings["Kosaraju with Matrix"] = profile_kosaraju("Kosaraju with Matrix", [&]() { run_kosaraju_matrix(n, edges); });
    timings["Kosaraju with Vector List"] = profile_kosaraju("Kosaraju with Vector List", [&]() { run_kosaraju_vector_list(n, edges); });

    // Determine the fastest algorithm
    auto fastest = min_element(timings.begin(), timings.end(), [](const auto& left, const auto& right) {
        return left.second < right.second;
    });

    cout << "\nThe fastest algorithm is " << fastest->first << " with a time of " << fastest->second << " seconds.\n";

    return 0;
}

/* 
USING GPROF:
1) Compile with pg: g++ -pg -o main your_code.cpp
2) Run the program: ./main
3) Analyze the profile data: gprof main gmon.out > analysis.txt
*/
