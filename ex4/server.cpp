#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm> // for find()
#include <mutex> // for thread synchronization
#include <netinet/in.h> // for the internet operations
#include <unistd.h> // for read(), write(), close()
#include <sys/types.h> // for types like socklen_t
#include <sys/socket.h> //  for socket operations
#include <sys/select.h> // for select
#include <sstream> // used to capture and manipulate string output in a flexible way
#include "../ex3/kosaraju_vector_list.hpp"

using namespace std;

/// Mutex for synchronizing access to the graph
// used to ensure that only one thread can access or modify the graph object at any given time.
mutex graphMutex;

/// Pointer to the current graph
KosarajuVectorList *graph = nullptr;

/// @brief Processes commands received from the client.
/// @param clientSocket The socket of the client.
/// @param command The command received from the client.
void processCommand(int clientSocket, const string& command) {
    string response;
    static int edgesToReceive = 0; // Number of edges to receive
    static vector<pair<int, int>> edges; // Vector to store edges

    if (command.find("NewGraph") == 0) {
        int n, m;
        //  reads input from a string
        // c_str() - Converts the command string (which is a std::string) to a C-style string (a const char*).
        sscanf(command.c_str(), "NewGraph %d %d", &n, &m);
        edgesToReceive = m; // Set the number of edges to receive
        edges.clear(); // Clear the edges vector
        edges.resize(m); // Resize the edges vector
        response = "Creating new graph...\n";
        response += "Number of vertices: " + to_string(n) + ", Number of edges: " + to_string(m) + "\n";
        response += "Please provide the edges one by one:\n";
        write(clientSocket, response.c_str(), response.size()); // Send response to client
    } else if (edgesToReceive > 0) { //  executed if the server is expecting edges to complete the graph creation.
        int u, v;
        sscanf(command.c_str(), "%d %d", &u, &v);
        edges[edges.size() - edgesToReceive] = {u, v}; // Store the edge
        response = "Edge " + to_string(edges.size() - edgesToReceive + 1) + ": " + to_string(u) + " -> " + to_string(v) + "\n";
        write(clientSocket, response.c_str(), response.size()); // Send response to client
        edgesToReceive--; // Decrease the number of edges to receive
        if (edgesToReceive == 0) { // Checks if all expected edges have been received.
            graphMutex.lock(); // Lock the graph mutex
            delete graph; // Delete the existing graph
            graph = new KosarajuVectorList(edges.size(), edges); // Create a new graph
            graphMutex.unlock(); // Unlock the graph mutex
            response = "Graph created successfully with " + to_string(edges.size()) + " vertices and " + to_string(edges.size()) + " edges\n";
            write(clientSocket, response.c_str(), response.size()); // Send response to client

            stringstream ss;
            streambuf* coutbuf = cout.rdbuf(); // Save old buffer
            cout.rdbuf(ss.rdbuf()); // Redirect cout to stringstream
            graph->printGraph(); // Print the graph
            cout.rdbuf(coutbuf); // Reset cout to its old buffer
            response = ss.str(); // extracts the content of the stringstream ss as a std::string
            write(clientSocket, response.c_str(), response.size()); // Send graph structure to client

            cout << "Graph created with " << edges.size() << " vertices and " << edges.size() << " edges" << endl;
        }
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
            cout << "Kosaraju algorithm executed" << endl;
        }
        graphMutex.unlock(); // Unlock the graph mutex
    } else if (command.find("NewEdge") == 0) {
        int u, v;
        sscanf(command.c_str(), "NewEdge %d %d", &u, &v);
        graphMutex.lock(); // Lock the graph mutex
        if (graph) {
            graph->addEdge(u, v); // Add the edge
            response = "Edge added successfully: " + to_string(u) + " -> " + to_string(v) + "\n";
            write(clientSocket, response.c_str(), response.size()); // Send response to client
            cout << "Edge added: " << u << " -> " << v << endl;
        }
        graphMutex.unlock(); // Unlock the graph mutex
    } else if (command.find("RemoveEdge") == 0) {
        int u, v;
        sscanf(command.c_str(), "RemoveEdge %d %d", &u, &v);
        graphMutex.lock(); // Lock the graph mutex
        if (graph) {
            graph->removeEdge(u, v); // Remove the edge
            response = "Edge removed successfully: " + to_string(u) + " -> " + to_string(v) + "\n";
            write(clientSocket, response.c_str(), response.size()); // Send response to client
            cout << "Edge removed: " << u << " -> " << v << endl;
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
    fd_set masterSet, readSet; // File descriptor sets for select.
    int fdMax;

    /* High-level of struct sockaddr_in:

    struct sockaddr_in {
        short            sin_family;   // address family, e.g. AF_INET
        unsigned short   sin_port;     //  port number for the socket
        struct in_addr   sin_addr;     //  structure to hold the IPv4 address(binary representation of the IP address for the socket)
        char             sin_zero[8];  // so it will be competible with the generic struct sockaddr
    };

    struct in_addr {
        unsigned long s_addr; //where we hold the ip address 
    };
    */

    /*
    AF_INET: Specifies the address family (IPv4). we chose IPv4 because it ensures compatibility with a vast majority of existing networks and devices.
    SOCK_STREAM: Specifies the socket type (TCP).
    0: means that the system should automatically choose the appropriate protocol for the given socket type. For SOCK_STREAM with AF_INET, this will be TCP..
    */
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if (serverSocket < 0) {
        cerr << "Error opening socket" << endl;
        return 1;
    }

    // Sets socket options. SO_REUSEADDR allows the socket to be quickly reused.
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { // Set socket options
        cerr << "Error setting socket options" << endl;
        return 1;
    }

    bzero((char*)&serverAddr, sizeof(serverAddr)); // Clears the server address structure.

    //we chose AF_INET as the address family for the socket(specifying that the socket will use IPv4 addresses, a protocol for transmitting data over the internet)
    //there are many protocols in AF_INET, among them the most popular are TCP and UDP. we will use TCP.
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 in IPv4, it will accept connections on all available networks.
    serverAddr.sin_port = htons(9034); //convert the representation of the port into big-endian(the expected network byte order).

    /* bind is associating a socket with a specific local address and port number
    serverSocket - file descriptor of the socket.
    (struct sockaddr *)&serverAddr - pointer to a sockaddr structure that contains the address to which the socket will bound.
    sizeof(serverAddr) - size of the server_addr structure, tells the bind how much memory to read.
    */
   if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { // Bind the socket
        cerr << "Error on binding" << endl;
        return 1;
    }

    /* tells the operating system that the program is ready to accept incoming connections on the socket sockfd.
    serverSocket - file descriptor of the socket.
    1 - backlog parameter, 
    When a client attempts to connect to the server, the connection is placed in a queue if the server is not immediately ready to accept it. 
    The backlog parameter determines the size of this queue.
    */
    listen(serverSocket, 1); // Listen for connections
    cout << "Server started on port 9034" << endl;

    // Initializes the file descriptor sets.
    FD_ZERO(&masterSet);
    FD_ZERO(&readSet);

    FD_SET(serverSocket, &masterSet); // Adds the server socket to the master set.
    fdMax = serverSocket;

    while (true) {
        readSet = masterSet;

        // Wait for input on either file descriptor
        if (select(fdMax + 1, &readSet, nullptr, nullptr, nullptr) == -1) { // Monitor sockets for activity
            cerr << "Error on select" << endl;
            return 1;
        }

        for (int i = 0; i <= fdMax; ++i) { // Iterates through the file descriptors.
            if (FD_ISSET(i, &readSet)) { // if a file descriptor is ready for reading
                if (i == serverSocket) {

                    /* Accept New Connection:
                    serverSocket: The listening socket that is ready to accept a new connection.
                    clientAddr: A sockaddr_in structure that will be filled with the address of the connecting entity (client).
                    addrLen: A variable that initially contains the size of clientAddr and is modified to the actual size of the address structure returned.
                    Returns: A new socket descriptor (clientSocket) for the accepted connection. This new socket is used for communication with the connected client.
                    */
                    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen); // Accept new connection
                    if (clientSocket == -1) {
                        cerr << "Error on accept" << endl;
                    } else {
                        FD_SET(clientSocket, &masterSet); // Add new socket to master set
                        if (clientSocket > fdMax) {
                            fdMax = clientSocket;
                        }
                        cout << "New connection on socket " << clientSocket << endl;
                    }
                } else { // i is not the server socket. Hence, i must be a client socket that has data ready to be read.
                    char buffer[1024];
                    bzero(buffer, 1024); //  Sets all bytes in the buffer to zero
                    int nbytes = read(i, buffer, 1023); // Read data from client
                    if (nbytes <= 0) {
                        if (nbytes == 0) {
                            cout << "Socket " << i << " hung up" << endl;
                        } else {
                            cerr << "Error on read" << endl;
                        }
                        close(i); // Close the socket
                        FD_CLR(i, &masterSet); // Remove socket from master set
                    } else {
                        processCommand(i, string(buffer)); // Process the command from client
                    }
                }
            }
        }
    }

    close(serverSocket); // Close the server socket
    return 0;
}
