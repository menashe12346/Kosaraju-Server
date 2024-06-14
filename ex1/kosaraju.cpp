#include <iostream>
#include <stack>
#include <vector>
#include <list>

using namespace std;

// Node class for the linked list
class Node {
public:
    int data;
    Node* next;
    Node(int data) : data(data), next(nullptr) {}
};

// Linked list class to manage the list operations
class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    void push(int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void print() const {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

// KosarajuList Implementation
class KosarajuList {
public:
    KosarajuList(int n, const vector<pair<int, int>>& edges) : n(n) {
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

    void findSCCs() {
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

    void printSCCs() const {
        cout << "Strongly Connected Components (SCCs):" << endl;
        int sccCount = 1;
        for (const auto& scc : sccs) {
            cout << "SCC " << sccCount++ << ": ";
            scc.print();
            cout << "----------------" << endl;
        }
    }

private:
    int n;
    vector<Node*> graph;
    vector<Node*> transposedGraph;
    vector<bool> visited;
    stack<int> finishStack;
    vector<LinkedList> sccs;

    void addEdge(Node*& head, int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    void dfsFirstPass(int node) {
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

    void dfsSecondPass(int node, LinkedList& scc) {
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
};

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

    KosarajuList kosaraju(n, edges);
    kosaraju.findSCCs();
    kosaraju.printSCCs();
    cout << "Done!" << endl;

    return 0;
}
