#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include "kosaraju_deque.hpp"
#include "kosaraju_list.hpp"
#include "kosaraju_matrix.hpp"
#include "kosaraju_vector_list.hpp"

using namespace std;
using namespace std::chrono;

template <typename Func>
void profile_kosaraju(const std::string& name, Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    cout << "Time taken by " << name << ": " << duration.count() << " seconds\n";
}

void run_kosaraju_deque(int n, const vector<pair<int, int>>& edges) {
    KosarajuDeque kosarajuDeque(n, edges);
    kosarajuDeque.findSCCs();
}

void run_kosaraju_list(int n, const vector<pair<int, int>>& edges) {
    KosarajuList kosarajuList(n, edges);
    kosarajuList.findSCCs();
}

void run_kosaraju_matrix(int n, const vector<pair<int, int>>& edges) {
    KosarajuMatrix kosarajuMatrix(n, edges);
    kosarajuMatrix.findSCCs();
}

void run_kosaraju_vector_list(int n, const vector<pair<int, int>>& edges) {
    KosarajuVectorList kosarajuVectorList(n, edges);
    kosarajuVectorList.findSCCs();
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

    profile_kosaraju("Kosaraju with Deque", [&]() { run_kosaraju_deque(n, edges); });
    profile_kosaraju("Kosaraju with List", [&]() { run_kosaraju_list(n, edges); });
    profile_kosaraju("Kosaraju with Matrix", [&]() { run_kosaraju_matrix(n, edges); });
    profile_kosaraju("Kosaraju with Vector List", [&]() { run_kosaraju_vector_list(n, edges); });

    return 0;
}
