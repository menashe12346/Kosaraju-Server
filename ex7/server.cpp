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
#include "../ex3/kosaraju_vector_list.hpp"

using namespace std;

/// Mutex for synchronizing access to the graph
mutex graphMutex;

/// Pointer to the current graph
KosarajuVectorList* graph = nullptr;

/// @brief Processes commands received from the client.
/// @param clientSocket The socket of the client.
/// @param command The command received from the client.
void processCommand(int clientSocket, const string& command);

/// @brief Handles a connected client in a separate thread.
/// @param arg Pointer to the client socket file descriptor.
void* handleClient(void* arg) {
    int clientSocket = *(int*)arg; // Dereference the client socket
    delete (int*)arg; // Free the allocated memory for the client socket

    char buffer[1024];
    bzero(buffer, 1024); // Clear the buffer
    int nbytes;
    while ((nbytes = read(clientSocket, buffer, 1023)) > 0) { // Read data from client
        processCommand(clientSocket, string(buffer)); // Process the command
        bzero(buffer, 1024); // Clear the buffer for the next read
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
            bzero(buffer, 1024); // Clear the buffer
            read(clientSocket, buffer, 1023); // Read edge from client
            sscanf(buffer, "%d %d", &edges[i].first, &edges[i].second); // Parse the edge
            response = "Edge " + to_string(i + 1) + ": " + to_string(edges[i].first) + " -> " + to_string(edges[i].second) + "\n";
            write(clientSocket, response.c_str(), response.size()); // Send edge information back to client
        }
        
        graphMutex.lock(); // Lock the graph mutex
        delete graph; // Delete the existing graph
        graph = new KosarajuVectorList(n, edges); // Create a new graph with the provided edges
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

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr; // hold the server and client address information
    socklen_t addrLen = sizeof(clientAddr);

    /* High-level overview of struct sockaddr_in:
    struct sockaddr_in {
        short            sin_family;   // address family, e.g. AF_INET
        unsigned short   sin_port;     // port number for the socket
        struct in_addr   sin_addr;     // structure to hold the IPv4 address (binary representation of the IP address for the socket)
        char             sin_zero[8];  // padding to make the structure the same size as struct sockaddr
    };

    struct in_addr {
        unsigned long s_addr; // holds the IP address 
    };
    */

    /*
    AF_INET: Specifies the address family (IPv4). We chose IPv4 because it ensures compatibility with a vast majority of existing networks and devices.
    SOCK_STREAM: Specifies the socket type (TCP).
    0: Automatically chooses the appropriate protocol for the given socket type. For SOCK_STREAM with AF_INET, this will be TCP.
    */
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if (serverSocket < 0) {
        cerr << "Error opening socket" << endl; // Log error if socket creation fails
        return 1;
    }

    // Sets socket options. SO_REUSEADDR allows the socket to be quickly reused.
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { // Set socket options
        cerr << "Error setting socket options" << endl; // Log error if setting options fails
        return 1;
    }

    bzero((char*)&serverAddr, sizeof(serverAddr)); // Clear the server address structure

    // Initialize server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 in IPv4, it will accept connections on all available networks.
    serverAddr.sin_port = htons(9034); // Convert the port number to network byte order

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { // Bind the socket
        cerr << "Error on binding" << endl; // Log error if binding fails
        return 1;
    }

    listen(serverSocket, 5); // Listen for connections
    cout << "Server started on port 9034" << endl; // Log server start

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen); // Accept new connection
        if (clientSocket == -1) {
            cerr << "Error on accept" << endl; // Log error if accept fails
        } else {
            cout << "New connection on socket " << clientSocket << endl; // Log new connection
            pthread_t threadId;
            int* pclient = new int;
            *pclient = clientSocket;
            pthread_create(&threadId, nullptr, handleClient, pclient); // Create a new thread to handle the client
            pthread_detach(threadId); // Detach the thread so that it cleans up after itself
        }
    }

    close(serverSocket); // Close the server socket
    return 0;
}
