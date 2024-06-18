#include "kosaraju_linked_list.hpp"

// Node class implementation
Node::Node(int data) : data(data), next(nullptr) {}

// LinkedList class implementation
LinkedList::LinkedList() : head(nullptr) {}

void LinkedList::push(int data) {
    Node* newNode = new Node(data);  // Create a new node with the given data
    newNode->next = head;  // Point the new node's next to the current head
    head = newNode;  // Update the head to the new node
}

void LinkedList::clear() {
    while (head) {
        Node* temp = head;  // Store the current head in a temporary pointer
        head = head->next;  // Move the head to the next node
        delete temp;  // Delete the old head
    }
}

void LinkedList::print() const {
    Node* current = head;  // Start from the head of the list
    while (current) {
        cout << current->data << " ";  // Print the current node's data
        current = current->next;  // Move to the next node
    }
    cout << endl;
}

// KosarajuList class implementation
KosarajuLinkedList::KosarajuLinkedList(int n, const vector<pair<int, int>>& edges) : n(n) {
    graph.resize(n + 1);  // Resize the graph to hold n nodes
    transposedGraph.resize(n + 1);  // Resize the transposed graph
    visited.resize(n + 1, false);  // Initialize visited array with false
    for (const auto& edge : edges) {
        if (edge.first > 0 && edge.first <= n && edge.second > 0 && edge.second <= n) {
            addEdge(graph[edge.first], edge.second);  // Add edge to the graph
            addEdge(transposedGraph[edge.second], edge.first);  // Add edge to the transposed graph
        } else {
            cerr << "Invalid edge: (" << edge.first << ", " << edge.second << ")" << endl;
        }
    }
}

const std::vector<LinkedList>& KosarajuLinkedList::getSCCs() const {
    return sccs;
}

/* STEPS: 
        1) DFS(G)
        2) DFS(Transposed G) based on the results of (1)
        3) Print the SCCs
*/
void KosarajuLinkedList::findSCCs() {
    sccs.clear(); // Clear the SCCs vector before finding SCCs
    fill(visited.begin(), visited.end(), false); // Reset the visited vector
    while (!finishStack.empty()) {
        finishStack.pop(); // Clear the finish stack
    }

    // **First Depth-First Search (DFS) Pass:**
    // - Iterate through all nodes.
    // - For each unvisited node, perform a DFS to explore all reachable nodes.
    // - Push each node onto a stack once all its descendants are visited (finish time stack).

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfsFirstPass(i);  // Perform DFS to fill the finish stack
        }
    }

    // **Reset Visited Array:**
    // - Clear the visited array to prepare for the second DFS pass.

    fill(visited.begin(), visited.end(), false);  // Reset the visited array for the second pass

    // **Second Depth-First Search (DFS) Pass on Transposed Graph:**
    // - Pop nodes from the stack one by one.
    // - For each unvisited node, perform a DFS on the transposed graph to explore all reachable nodes, collecting nodes into a linked list representing an SCC.

    while (!finishStack.empty()) {
        int node = finishStack.top();  // Get the top node from the finish stack
        finishStack.pop();  // Remove the top node from the stack
        if (!visited[node]) {
            LinkedList scc;  // Create a new linked list to store the SCC
            dfsSecondPass(node, scc);  // Perform DFS to collect nodes in the SCC
            sccs.push_back(scc);  // Add the SCC to the list of SCCs
        }
    }
}

void KosarajuLinkedList::printSCCs() const {
    cout << "\nKosaraju Linked List algorithm: Strongly Connected Components (SCCs):" << endl;
    int sccCount = 1;
    for (const auto& scc : sccs) {
        cout << "SCC " << sccCount++ << ": ";
        
        // To print the linked list from end to beginning, we need to collect elements first
        vector<int> elements;
        Node* current = scc.head;
        while (current) {
            elements.push_back(current->data);
            current = current->next;
        }
        
        // Print elements in reverse order
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            cout << *it << " ";
        }
        
        cout << endl << "----------------" << endl;
    }
}


void KosarajuLinkedList::addEdge(Node*& head, int data) {
    Node* newNode = new Node(data);  // Create a new node with the given data
    newNode->next = head;  // Point the new node's next to the current head
    head = newNode;  // Update the head to the new node
}

void KosarajuLinkedList::dfsFirstPass(int node) {
    visited[node] = true;  // Mark the node as visited
    Node* current = graph[node];  // Get the adjacency list of the node
    while (current) {
        if (!visited[current->data]) {
            dfsFirstPass(current->data);  // Recursively visit all unvisited adjacent nodes
        }
        current = current->next;  // Move to the next adjacent node
    }
    finishStack.push(node);  // Push the node onto the finish stack after visiting all its descendants
}

void KosarajuLinkedList::dfsSecondPass(int node, LinkedList& scc) {
    visited[node] = true;  // Mark the node as visited
    scc.push(node);  // Add the node to the current SCC
    Node* current = transposedGraph[node];  // Get the adjacency list of the node in the transposed graph
    while (current) {
        if (!visited[current->data]) {
            dfsSecondPass(current->data, scc);  // Recursively visit all unvisited adjacent nodes
        }
        current = current->next;  // Move to the next adjacent node
    }
}
