#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm>
#include <mutex>
#include <queue>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sstream>
#include "kosaraju_vector_list.hpp"
#include "../ex8/reactor.hpp"

using namespace std;

mutex graphMutex;
KosarajuVectorList* graph = nullptr;
pthread_cond_t conditionVar = PTHREAD_COND_INITIALIZER;
pthread_cond_t queueConditionVar = PTHREAD_COND_INITIALIZER;
mutex queueMutex;
queue<pair<int, string>> commandQueue;
bool computationDone = false;
bool sccConditionMet = false;
bool isServerRunning = true;

void processCommand(int clientSocket, const string& command);

void* handleClient(void* arg) {
    int clientSocket = *(int*)arg;
    delete (int*)arg;

    char buffer[1024];
    memset(buffer, 0, 1024);
    int nbytes;
    while ((nbytes = read(clientSocket, buffer, 1023)) > 0) {
        unique_lock<mutex> lock(queueMutex);
        commandQueue.push(make_pair(clientSocket, string(buffer)));
        pthread_cond_signal(&queueConditionVar);
    }
    
    if (nbytes == 0) {
        cout << "Socket " << clientSocket << " hung up" << endl;
    } else {
        cerr << "Error on read" << endl;
    }

    close(clientSocket);
    return nullptr;
}

void checkGraphSCC() {
    graphMutex.lock();
    if (graph) {
        graph->findSCCs();
        vector<vector<int>> sccs = graph->getSCCs();
        int maxSCCSize = 0;
        for (const auto& scc : sccs) {
            if (scc.size() > maxSCCSize) {
                maxSCCSize = scc.size();
            }
        }
        bool newSccConditionMet = maxSCCSize >= graph->getNumVertices() / 2;
        if (newSccConditionMet != sccConditionMet) {
            sccConditionMet = newSccConditionMet;
            if (sccConditionMet) {
                cout << "At Least 50% of the graph belongs to the same SCC" << endl;
            } else {
                cout << "At Least 50% of the graph no longer belongs to the same SCC" << endl;
            }
            pthread_cond_broadcast(&conditionVar);
        }
        graphMutex.unlock();
    } else {
        graphMutex.unlock();
    }
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
            memset(buffer, 0, 1024);
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

        // Check the graph SCC after Kosaraju execution
        checkGraphSCC();
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

        // Check the graph SCC after adding an edge
        checkGraphSCC();
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

        // Check the graph SCC after removing an edge
        checkGraphSCC();
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

void* consumerThread(void* arg) {
    while (isServerRunning) {
        unique_lock<mutex> lock(queueMutex);
        while (commandQueue.empty() && isServerRunning) {
            pthread_cond_wait(&queueConditionVar, &queueMutex.native_handle());
        }

        if (!isServerRunning) {
            break;
        }

        auto commandPair = commandQueue.front();
        commandQueue.pop();
        lock.unlock();

        processCommand(commandPair.first, commandPair.second);
    }

    return nullptr;
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

    memset((char*)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9034);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error on binding" << endl;
        return 1;
    }

    listen(serverSocket, 5);
    cout << "Server started on port 9034" << endl;

    pthread_t consumerTid;
    pthread_create(&consumerTid, nullptr, consumerThread, nullptr);

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            cerr << "Error on accept" << endl;
        } else {
            cout << "New connection on socket " << clientSocket << endl;
            int* pclient = new int(clientSocket);
            pthread_t tid = startProactor(clientSocket, handleClient);
            pthread_detach(tid); // Detach the thread so that it cleans up after itself
        }
    }

    isServerRunning = false;
    pthread_cond_broadcast(&queueConditionVar);
    pthread_join(consumerTid, nullptr);

    close(serverSocket);
    return 0;
}
