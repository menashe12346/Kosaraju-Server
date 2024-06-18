#include <iostream>
#include <netinet/in.h> // for sockaddr_in structure
#include <unistd.h> // for bzero, close
#include <cstring> // for bzero
#include <arpa/inet.h> // for inet_addr

using namespace std;

/// @brief Sends a command to the server.
/// @param socket The socket of the server.
/// @param command The command to send to the server.
void sendCommand(int socket, const string& command) {
    // Writes the command to the specified socket. The command is converted to a C-style string using c_str().
    if (write(socket, command.c_str(), command.size()) < 0) {
        cerr << "Error writing to socket" << endl;
    }
}

/// @brief Receives a response from the server and prints it to the console.
/// @param socket The socket of the server.
void receiveResponse(int socket) {
    char buffer[1024];
    bzero(buffer, 1024); // Clears the buffer.
    if (read(socket, buffer, 1023) < 0) { // Reads from the socket into the buffer.
        cerr << "Error reading from socket" << endl;
    } else {
        cout << buffer;
    }
}

/// @brief Prints a help message with available commands and their descriptions.
void printHelp() {
    cout << "\nAvailable commands:\n"
         << "NewGraph n m\n"
         << "  - Create a new graph with n vertices and m edges\n"
         << "  - Example: NewGraph 5 5\n"
         << "  - After this command, provide the edges one by one:\n"
         << "    1 2\n"
         << "    2 3\n"
         << "    3 4\n"
         << "    4 5\n"
         << "    5 1\n"
         << "NewEdge u v\n"
         << "  - Add a new edge from vertex u to vertex v\n"
         << "  - Example: NewEdge 3 4\n"
         << "RemoveEdge u v\n"
         << "  - Remove an edge from vertex u to vertex v\n"
         << "  - Example: RemoveEdge 3 4\n"
         << "Kosaraju\n"
         << "  - Run the Kosaraju algorithm to find strongly connected components\n"
         << "  - Example: Kosaraju\n"
         << "PrintGraph\n"
         << "  - Print the current graph\n"
         << "  - Example: PrintGraph\n"
         << "exit\n"
         << "  - Exit the client\n"
         << "  - Example: exit\n"
         << "help\n"
         << "  - Display this help message\n"
         << "  - Example: help\n"
         << endl;
}

int main() {
    int sockfd, portno = 9034; // the port of beej
    struct sockaddr_in serv_addr;

    /* High-level overview of struct sockaddr_in:

    struct sockaddr_in {
        short            sin_family;   // address family, e.g., AF_INET
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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error opening socket" << endl;
        return 1;
    }
    
    // We chose AF_INET as the address family for the socket (specifying that the socket will use IPv4 addresses, a protocol for transmitting data over the internet).
    // There are many protocols in AF_INET, among them the most popular are TCP and UDP. We will use TCP.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use localhost for testing
    serv_addr.sin_port = htons(portno); // Convert the representation of the port into big-endian (the expected network byte order).
    
    // Attempts to connect to the server specified by serv_addr.
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error connecting" << endl;
        return 1;
    }
    
    while (true) {
        cout << "Enter command (NewGraph, NewEdge, RemoveEdge, Kosaraju, PrintGraph, help, exit): ";
        string command;
        getline(cin, command); // Reads the entire input line into the command string.
        
        if (command == "help") {
            printHelp();
            continue;
        }
        
        sendCommand(sockfd, command + "\n");
        receiveResponse(sockfd);
        if (command == "exit") break;
        
        if (command.find("NewGraph") == 0) {
            int n, m;
            sscanf(command.c_str(), "NewGraph %d %d", &n, &m);
            for (int i = 0; i < m; ++i) {
                cout << "Enter edge " << i + 1 << ": ";
                getline(cin, command);
                sendCommand(sockfd, command + "\n");
                receiveResponse(sockfd);
            }
        }
    }
    
    close(sockfd);
    return 0;
}
