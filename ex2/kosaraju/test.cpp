#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "kosaraju_deque.hpp"
#include "kosaraju_list.hpp"
#include "kosaraju_matrix.hpp"
#include "kosaraju_vector_list.hpp"
#include "../../ex1/kosaraju_linked_list.hpp"

using namespace std;

// Helper function to convert list to vector
vector<int> listToVector(const list<int>& lst) {
    return vector<int>(lst.begin(), lst.end());
}

// Helper function to convert LinkedList to vector
vector<int> linkedListToVector(const LinkedList& linkedList) {
    vector<int> vec;
    Node* current = linkedList.head;
    while (current) {
        vec.push_back(current->data);
        current = current->next;
    }
    return vec;
}

// Helper function to compare SCCs from different implementations
bool compare_scc(const deque<deque<int>>& scc1,
                 const list<list<int>>& scc2,
                 const vector<vector<int>>& scc3,
                 const vector<vector<int>>& scc4,
                 const vector<LinkedList>& scc5,
                 vector<vector<int>> expected_scc) {
    if (scc1.size() != expected_scc.size() || scc2.size() != expected_scc.size() ||
        scc3.size() != expected_scc.size() || scc4.size() != expected_scc.size() ||
        scc5.size() != expected_scc.size()) {
        cout << "Size mismatch:" << endl;
        cout << "scc1.size() = " << scc1.size() << ", expected_scc.size() = " << expected_scc.size() << endl;
        cout << "scc2.size() = " << scc2.size() << ", expected_scc.size() = " << expected_scc.size() << endl;
        cout << "scc3.size() = " << scc3.size() << ", expected_scc.size() = " << expected_scc.size() << endl;
        cout << "scc4.size() = " << scc4.size() << ", expected_scc.size() = " << expected_scc.size() << endl;
        cout << "scc5.size() = " << scc5.size() << ", expected_scc.size() = " << expected_scc.size() << endl;
        return false;
    }

    // Sort each SCC in expected_scc
    for (auto& scc : expected_scc) {
        sort(scc.begin(), scc.end());
    }

    auto it1 = scc1.begin();
    auto it2 = scc2.begin();
    auto it3 = scc3.begin();
    auto it4 = scc4.begin();
    auto it5 = scc5.begin();

    for (const auto& expected : expected_scc) {
        vector<int> sorted_scc1(it1->begin(), it1->end());
        sort(sorted_scc1.begin(), sorted_scc1.end());

        vector<int> sorted_scc2 = listToVector(*it2);
        sort(sorted_scc2.begin(), sorted_scc2.end());

        vector<int> sorted_scc3 = *it3;
        sort(sorted_scc3.begin(), sorted_scc3.end());

        vector<int> sorted_scc4 = *it4;
        sort(sorted_scc4.begin(), sorted_scc4.end());

        vector<int> sorted_scc5 = linkedListToVector(*it5);
        sort(sorted_scc5.begin(), sorted_scc5.end());

        if (sorted_scc1 != expected || sorted_scc2 != expected ||
            sorted_scc3 != expected || sorted_scc4 != expected ||
            sorted_scc5 != expected) {
            cout << "Mismatch found:" << endl;
            cout << "Expected: ";
            for (const int& val : expected) cout << val << " ";
            cout << endl;
            cout << "Deque: ";
            for (const int& val : sorted_scc1) cout << val << " ";
            cout << endl;
            cout << "List: ";
            for (const int& val : sorted_scc2) cout << val << " ";
            cout << endl;
            cout << "Matrix: ";
            for (const int& val : sorted_scc3) cout << val << " ";
            cout << endl;
            cout << "VectorList: ";
            for (const int& val : sorted_scc4) cout << val << " ";
            cout << endl;
            cout << "LinkedList: ";
            for (const int& val : sorted_scc5) cout << val << " ";
            cout << endl;
            return false;
        }

        ++it1;
        ++it2;
        ++it3;
        ++it4;
        ++it5;
    }
    return true;
}

void test_findSCC() {
    // Define edges of the graph
    vector<pair<int, int>> edges = {
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 3}
    };

    int num_vertices = 5;
    vector<vector<int>> expected_scc = {
        {1},
        {2},
        {3, 4, 5}
    };

    // Initialize the Kosaraju algorithms and find SCCs
    KosarajuDeque kosarajuDeque(num_vertices, edges);
    kosarajuDeque.findSCCs();

    KosarajuList kosarajuList(num_vertices, edges);
    kosarajuList.findSCCs();

    KosarajuMatrix kosarajuMatrix(num_vertices, edges);
    kosarajuMatrix.findSCCs();

    KosarajuVectorList kosarajuVectorList(num_vertices, edges);
    kosarajuVectorList.findSCCs();

    KosarajuLinkedList kosarajuLinkedList(num_vertices, edges);
    kosarajuLinkedList.findSCCs();

    // Compare SCCs and check if they match the expected result
    if (compare_scc(kosarajuDeque.getSCCs(), kosarajuList.getSCCs(),
                    kosarajuMatrix.getSCCs(), kosarajuVectorList.getSCCs(),
                    kosarajuLinkedList.getSCCs(), expected_scc)) {
        cout << "All Kosaraju Algorithms Test Passed!" << endl;
    } else {
        cout << "Kosaraju Algorithms Test Failed!" << endl;
    }

    kosarajuDeque.printSCCs();

    kosarajuList.printSCCs();

    kosarajuMatrix.printSCCs();

    kosarajuVectorList.printSCCs();

    kosarajuLinkedList.printSCCs();
}

int main() {
    test_findSCC();
    return 0;
}
