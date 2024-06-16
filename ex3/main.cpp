#include <iostream>
#include <vector>
#include <utility>
#include "kosaraju_vector_list.hpp"

using namespace std;

int main() {
    cout << "Reading number of vertices and edges..." << endl;
    int n, m;
    cin >> n >> m;
    cout << "Number of vertices: " << n << ", Number of edges: " << m << endl;
    vector<pair<int, int>> edges(m);

    cout << "Reading edges..." << endl;
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].first >> edges[i].second;
        cout << "Edge " << i << ": " << edges[i].first << " -> " << edges[i].second << endl;
    }

    KosarajuVectorList kosaraju(n, edges);
    
    char choice;
    while (true) {
        cout << "Do you want to add an edge? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            int u, v;
            cout << "Enter the edge to add (u v): ";
            cin >> u >> v;
            kosaraju.addEdge(u, v);
            cout << "Edge added: " << u << " -> " << v << endl;
        } else {
            break;
        }
    }

    while (true) {
        cout << "Do you want to remove an edge? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            int u, v;
            cout << "Enter the edge to remove (u v): ";
            cin >> u >> v;
            kosaraju.removeEdge(u, v);
            cout << "Edge removed: " << u << " -> " << v << endl;
        } else {
            break;
        }
    }

    kosaraju.findSCCs();
    kosaraju.printSCCs();
    cout << "Done!" << endl;

    return 0;
}
