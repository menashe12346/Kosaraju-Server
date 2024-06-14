#include "kosaraju.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

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

    cout << "Timing deque implementation..." << endl;

    // Time the deque implementation
    auto startDeque = high_resolution_clock::now();
    KosarajuDeque kosarajuDeque(n, edges);
    kosarajuDeque.findSCCs();
    auto endDeque = high_resolution_clock::now();
    auto durationDeque = duration_cast<milliseconds>(endDeque - startDeque).count();
    cout << "Deque Implementation took: " << durationDeque << " ms" << endl;

    cout << "Timing list implementation..." << endl;

    // Time the list implementation
    auto startList = high_resolution_clock::now();
    KosarajuList kosarajuList(n, edges);
    kosarajuList.findSCCs();
    auto endList = high_resolution_clock::now();
    auto durationList = duration_cast<milliseconds>(endList - startList).count();
    cout << "List Implementation took: " << durationList << " ms" << endl;

    // Print the SCCs found by the deque implementation
    cout << "Printing SCCs found by deque implementation..." << endl;
    kosarajuDeque.printSCCs();

    cout << "Done!" << endl;

    return 0;
}
