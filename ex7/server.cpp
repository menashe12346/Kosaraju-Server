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

mutex graphMutex;
KosarajuVectorList* graph = nullptr;

void processCommand(int clientSocket, const string& command);

void* handleClient(void* arg) {
    int clientSocket = *(int*)arg;
    delete (int*)arg;

    char buffer[1024];
    bzero(buffer, 1024);
    int nbytes;
    while ((nbytes = read(clientSocket, buffer, 1023)) > 0) {
        processCommand(clientSocket, string(buffer));
        bzero(buffer, 1024);
    }
    
    if (nbytes == 0) {
        cout << "Socket " << clientSocket << " hung up" << endl;
    } else {
        cerr << "Error on read" << endl;
    }

    close(clientSocket);
    return nullptr;
}

void processCommand(int clientSocket, const string& command) {
    string response;
    if (command.find("NewGraph") == 0) {
        int n, m;
        sscanf(command.c_str(), "NewGraph %d %d", &n, &m);
        vector<pair<int, int>> edges(m);
        response = "Creating new graph...\n";
        response += "Number of vertices: " + to_string(n) + ", Number of edges: " + to_string(m) + "\n";
        response += "Please provide the edges one by one:\n";
        write(clientSocket, response.c_str(), response.size());
        
        for (int i = 0; i < m; ++i) {
            char buffer[1024];
            bzero(buffer, 1024);
            read(clientSocket, buffer, 1023);
            sscanf(buffer, "%d %d", &edges[i].first, &edges[i].second);
            response = "Edge " + to_string(i + 1) + ": " + to_string(edges[i].first) + " -> " + to_string(edges[i].second) + "\n";
            write(clientSocket, response.c_str(), response.size());
        }
        
        graphMutex.lock();
        delete graph;
        graph = new KosarajuVectorList(n, edges);
        graphMutex.unlock();
        response = "Graph created successfully with " + to_string(n) + " vertices and " + to_string(m) + " edges\n";
        write(clientSocket, response.c_str(), response.size());

        stringstream ss;
        streambuf* coutbuf = cout.rdbuf();
        cout.rdbuf(ss.rdbuf());
        graph->printGraph();
        cout.rdbuf(coutbuf);
        response = ss.str();
        write(clientSocket, response.c_str(), response.size());

        cout << "Graph created with " << n << " vertices and " << m << " edges" << endl;
    } else if (command.find("Kosaraju") == 0) {
        graphMutex.lock();
        if (graph) {
            graph->findSCCs();
            stringstream ss;
            streambuf* coutbuf = cout.rdbuf();
            cout.rdbuf(ss.rdbuf());
            graph->printSCCs();
            cout.rdbuf(coutbuf);
            response = ss.str();
            response += "Kosaraju algorithm executed\n";
            write(clientSocket, response.c_str(), response.size());
            cout << "Kosaraju algorithm executed" << endl;
        }
        graphMutex.unlock();
    } else if (command.find("NewEdge") == 0) {
        int u, v;
        sscanf(command.c_str(), "NewEdge %d %d", &u, &v);
        graphMutex.lock();
        if (graph) {
            graph->addEdge(u, v);
            response = "Edge added successfully: " + to_string(u) + " -> " + to_string(v) + "\n";
            write(clientSocket, response.c_str(), response.size());
            cout << "Edge added: " << u << " -> " << v << endl;
        }
        graphMutex.unlock();
    } else if (command.find("RemoveEdge") == 0) {
        int u, v;
        sscanf(command.c_str(), "RemoveEdge %d %d", &u, &v);
        graphMutex.lock();
        if (graph) {
            graph->removeEdge(u, v);
            response = "Edge removed successfully: " + to_string(u) + " -> " + to_string(v) + "\n";
            write(clientSocket, response.c_str(), response.size());
            cout << "Edge removed: " << u << " -> " << v << endl;
        }
        graphMutex.unlock();
    } else if (command.find("PrintGraph") == 0) {
        graphMutex.lock();
        if (graph) {
            stringstream ss;
            streambuf* coutbuf = cout.rdbuf();
            cout.rdbuf(ss.rdbuf());
            graph->printGraph();
            cout.rdbuf(coutbuf);
            response = ss.str();
            write(clientSocket, response.c_str(), response.size());
        }
        graphMutex.unlock();
    } else if (command.find("exit") == 0) {
        response = "Exiting...\n";
        write(clientSocket, response.c_str(), response.size());
    } else {
        response = "Invalid command\n";
        write(clientSocket, response.c_str(), response.size());
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error opening socket" << endl;
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "Error setting socket options" << endl;
        return 1;
    }

    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9034);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error on binding" << endl;
        return 1;
    }

    listen(serverSocket, 5);
    cout << "Server started on port 9034" << endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            cerr << "Error on accept" << endl;
        } else {
            cout << "New connection on socket " << clientSocket << endl;
            pthread_t threadId;
            int* pclient = new int;
            *pclient = clientSocket;
            pthread_create(&threadId, nullptr, handleClient, pclient);
            pthread_detach(threadId); // Detach the thread so that it cleans up after itself
        }
    }

    close(serverSocket);
    return 0;
}
