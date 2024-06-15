#include "kosaraju_linked_list.hpp"

// Node class implementation
Node::Node(int data) : data(data), next(nullptr) {}

// LinkedList class implementation
LinkedList::LinkedList() : head(nullptr) {}

void LinkedList::push(int data) {
    Node* newNode = new Node(data);
    newNode->next = head;
    head = newNode;
}

void LinkedList::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void LinkedList::print() const {
    Node* current = head;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

// KosarajuList class implementation
KosarajuLinkedList::KosarajuLinkedList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n + 1);
    transposedGraph.resize(n + 1);
    visited.resize(n + 1, false);
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            addEdge(graph[edge.first], edge.second);
            addEdge(transposedGraph[edge.second], edge.first);
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;
        }
    }
}

void KosarajuLinkedList::findSCCs() {
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfsFirstPass(i);
        }
    }

    fill(visited.begin(), visited.end(), false);
    while (!finishStack.empty()) {
        int node = finishStack.top();
        finishStack.pop();
        if (!visited[node]) {
            LinkedList scc;
            dfsSecondPass(node, scc);
            sccs.push_back(scc);
        }
    }
}

void KosarajuLinkedList::printSCCs() const {
    cout << "\nKosaraju Linked List algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        scc.print();
        cout << "----------------" << endl;
    }
}

void KosarajuLinkedList::addEdge(Node*& head, int data) {
    Node* newNode = new Node(data);
    newNode->next = head;
    head = newNode;
}

void KosarajuLinkedList::dfsFirstPass(int node) {
    visited[node] = true;
    Node* current = graph[node];
    while (current) {
        if (!visited[current->data]) {
            dfsFirstPass(current->data);
        }
        current = current->next;
    }
    finishStack.push(node);
}

void KosarajuLinkedList::dfsSecondPass(int node, LinkedList& scc) {
    visited[node] = true;
    scc.push(node);
    Node* current = transposedGraph[node];
    while (current) {
        if (!visited[current->data]) {
            dfsSecondPass(current->data, scc);
        }
        current = current->next;
    }
}
