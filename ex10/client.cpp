#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

void sendCommand(int socket, const string& command) {
    write(socket, command.c_str(), command.size());
    cout << "Sent command: " << command << endl; // Debugging print
}

void receiveResponse(int socket) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    int nbytes = read(socket, buffer, 1023);
    if (nbytes > 0) {
        cout << "Received response: " << buffer;
    } else {
        cerr << "Error on read or connection closed" << endl;
    }
}

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
    int sockfd, portno = 9034;
    struct sockaddr_in serv_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error opening socket" << endl;
        return 1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error connecting" << endl;
        return 1;
    }
    
    while (true) {
        cout << "Enter command (NewGraph, NewEdge, RemoveEdge, Kosaraju, PrintGraph, help, exit): ";
        string command;
        getline(cin, command);
        
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
