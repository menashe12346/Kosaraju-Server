#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sstream>
#include <condition_variable>
#include "kosaraju_vector_list.hpp"
#include "../ex8/reactor.hpp"

using namespace std;

/// Mutex for synchronizing access to the graph
mutex graphMutex;
/// Condition variable for signaling the monitoring thread
condition_variable graphCondVar;
/// Pointer to the current graph
KosarajuVectorList* graph = nullptr;
/// Flag to indicate if 50% SCC condition is met
bool sccConditionMet = false;
bool sccConditionWasMet = false;

/// @brief Processes commands received from the client.
/// @param clientSocket The socket of the client.
/// @param command The command received from the client.
void processCommand(int clientSocket, const string& command);

/// @brief Handles a connected client.
/// @param clientSocket The socket of the client.
/// @return nullptr
void* handleClient(int clientSocket) {
    char buffer[1024];
    memset(buffer, 0, 1024); // Clear the buffer
    int nbytes;
    while ((nbytes = read(clientSocket, buffer, 1023)) > 0) { // Read data from client
        processCommand(clientSocket, string(buffer)); // Process the command
        memset(buffer, 0, 1024); // Clear the buffer for the next read
    }
    
    if (nbytes == 0) {
        cout << "Socket " << clientSocket << " hung up" << endl; // Log if the client disconnected
    } else {
        cerr << "Error on read" << endl; // Log read error
    }

    close(clientSocket); // Close the socket
    return nullptr;
}

/// @brief Processes commands received from the client.
/// @param clientSocket The socket of the client.
/// @param command The command received from the client.
void processCommand(int clientSocket, const string& command) {
    string response;
    if (command.find("NewGraph") == 0) {
        int n, m;
        sscanf(command.c_str(), "NewGraph %d %d", &n, &m); // Parse the number of vertices and edges
        vector<pair<int, int>> edges(m); // Create a vector to store edges
        response = "Creating new graph...\n";
        response += "Number of vertices: " + to_string(n) + ", Number of edges: " + to_string(m) + "\n";
        response += "Please provide the edges one by one:\n";
        write(clientSocket, response.c_str(), response.size()); // Send response to client
        
        for (int i = 0; i < m; ++i) { // Loop to receive edges from the client
            char buffer[1024];
            memset(buffer, 0, 1024); // Clear the buffer
            read(clientSocket, buffer, 1023); // Read edge from client
            sscanf(buffer, "%d %d", &edges[i].first, &edges[i].second); // Parse the edge
            response = "Edge " + to_string(i + 1) + ": " + to_string(edges[i].first) + " -> " + to_string(edges[i].second) + "\n";
            write(clientSocket, response.c_str(), response.size()); // Send edge information back to client
        }
        
        graphMutex.lock(); // Lock the graph mutex
        delete graph; // Delete the existing graph
        graph = new KosarajuVectorList(n, edges); // Create a new graph with the provided edges
        sccConditionMet = false; // Reset the SCC condition flag
        sccConditionWasMet = false; // Reset the previous SCC condition flag
        graphMutex.unlock(); // Unlock the graph mutex
        response = "Graph created successfully with " + to_string(n) + " vertices and " + to_string(m) + " edges\n";
        write(clientSocket, response.c_str(), response.size()); // Send confirmation to client

        stringstream ss;
        streambuf* coutbuf = cout.rdbuf(); // Save old buffer
        cout.rdbuf(ss.rdbuf()); // Redirect cout to stringstream
        graph->printGraph(); // Print the graph
        cout.rdbuf(coutbuf); // Reset cout to its old buffer
        response = ss.str(); // Get the string from stringstream
        write(clientSocket, response.c_str(), response.size()); // Send graph structure to client

        cout << "Graph created with " << n << " vertices and " << m << " edges" << endl; // Log to console
    } else if (command.find("Kosaraju") == 0) {
        graphMutex.lock(); // Lock the graph mutex
        if (graph) {
            graph->findSCCs(); // Find strongly connected components
            stringstream ss;
            streambuf* coutbuf = cout.rdbuf(); // Save old buffer
            cout.rdbuf(ss.rdbuf()); // Redirect cout to stringstream
            graph->printSCCs(); // Print SCCs
            cout.rdbuf(coutbuf); // Reset cout to its old buffer
            response = ss.str();
            response += "Kosaraju algorithm executed\n";
            write(clientSocket, response.c_str(), response.size()); // Send response to client
            cout << "Kosaraju algorithm executed" << endl; // Log to console

            // Check SCC condition
            int largestSCC = graph->largestSCCSize();
            bool previousConditionMet = sccConditionMet;
            sccConditionMet = largestSCC >= graph->getNumVertices() / 2;

            if (sccConditionMet != previousConditionMet) {
                graphCondVar.notify_all(); // Notify the monitoring thread
            }
        }
        graphMutex.unlock(); // Unlock the graph mutex
    } else if (command.find("NewEdge") == 0) {
        int u, v;
        sscanf(command.c_str(), "NewEdge %d %d", &u, &v); // Parse the edge to add
        graphMutex.lock(); // Lock the graph mutex
        if (graph) {
            graph->addEdge(u, v); // Add the edge
            response = "Edge added successfully: " + to_string(u) + " -> " + to_string(v) + "\n";
            write(clientSocket, response.c_str(), response.size()); // Send confirmation to client
            cout << "Edge added: " << u << " -> " << v << endl; // Log to console
        }
        graphMutex.unlock(); // Unlock the graph mutex
    } else if (command.find("RemoveEdge") == 0) {
        int u, v;
        sscanf(command.c_str(), "RemoveEdge %d %d", &u, &v); // Parse the edge to remove
        graphMutex.lock(); // Lock the graph mutex
        if (graph) {
            graph->removeEdge(u, v); // Remove the edge
            response = "Edge removed successfully: " + to_string(u) + " -> " + to_string(v) + "\n";
            write(clientSocket, response.c_str(), response.size()); // Send confirmation to client
            cout << "Edge removed: " << u << " -> " << v << endl; // Log to console
        }
        graphMutex.unlock(); // Unlock the graph mutex
    } else if (command.find("PrintGraph") == 0) {
        graphMutex.lock(); // Lock the graph mutex
        if (graph) {
            stringstream ss;
            streambuf* coutbuf = cout.rdbuf(); // Save old buffer
            cout.rdbuf(ss.rdbuf()); // Redirect cout to stringstream
            graph->printGraph(); // Print the graph
            cout.rdbuf(coutbuf); // Reset cout to its old buffer
            response = ss.str();
            write(clientSocket, response.c_str(), response.size()); // Send graph structure to client
        }
        graphMutex.unlock(); // Unlock the graph mutex
    } else if (command.find("exit") == 0) {
        response = "Exiting...\n";
        write(clientSocket, response.c_str(), response.size()); // Send response to client
    } else {
        response = "Invalid command\n";
        write(clientSocket, response.c_str(), response.size()); // Send response to client
    }
}

/// @brief Monitoring thread function.
/// @return nullptr
void* monitorGraph(void*) {
    unique_lock<mutex> lock(graphMutex);
    while (true) {
        graphCondVar.wait(lock, []{ return sccConditionMet != sccConditionWasMet; }); // Wait for SCC condition change
        if (sccConditionMet) {
            cout << "At Least 50% of the graph belongs to the same SCC\n";
        } else {
            cout << "At Least 50% of the graph no longer belongs to the same SCC\n";
        }
        sccConditionWasMet = sccConditionMet; // Update the previous condition state
    }
    return nullptr;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if (serverSocket < 0) {
        cerr << "Error opening socket" << endl; // Log error if socket creation fails
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "Error setting socket options" << endl; // Log error if setting options fails
        return 1;
    }

    memset((char*)&serverAddr, 0, sizeof(serverAddr)); // Clear the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 in IPv4, it will accept connections on all available networks.
    serverAddr.sin_port = htons(9034); // Convert the port number to network byte order

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { // Bind the socket
        cerr << "Error on binding" << endl; // Log error if binding fails
        return 1;
    }

    listen(serverSocket, 5); // Listen for connections
    cout << "Server started on port 9034" << endl; // Log server start

    // Start the monitoring thread
    pthread_t monitorThread;
    pthread_create(&monitorThread, nullptr, monitorGraph, nullptr);

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen); // Accept new connection
        if (clientSocket == -1) {
            cerr << "Error on accept" << endl; // Log error if accept fails
        } else {
            cout << "New connection on socket " << clientSocket << endl; // Log new connection
            pthread_t tid = startProactor(clientSocket, handleClient); // Start proactor thread for the client
            pthread_detach(tid); // Detach the thread so that it cleans up after itself
        }
    }

    close(serverSocket); // Close the server socket
    return 0;
}
