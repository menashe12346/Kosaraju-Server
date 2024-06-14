#include "kosaraju_vector_list.hpp"

#include "kosaraju_list.hpp"

#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include "kosaraju_deque.hpp"
#include "kosaraju_matrix.hpp"
using namespace std;

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

int main(){
    // Read the number of vertices and edges

}