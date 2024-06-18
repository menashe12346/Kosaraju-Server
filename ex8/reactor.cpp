#include "reactor.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring> // For memset

using namespace std;

void* startReactor() {
    Reactor* reactor = new Reactor();
    FD_ZERO(&reactor->masterSet); // Initialize master set of file descriptors
    FD_ZERO(&reactor->readSet); // Initialize read set for select
    reactor->fdMax = 0; // Initialize the maximum file descriptor value
    reactor->running = true; // Set running flag to true
    return reactor;
}

int addFdToReactor(void* reactorPtr, int fd, reactorFunc func) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    FD_SET(fd, &reactor->masterSet); // Add the file descriptor to the master set
    if (fd > reactor->fdMax) {
        reactor->fdMax = fd; // Update the maximum file descriptor value if needed
    }
    reactor->handlers[fd] = func; // Map the file descriptor to its handler function
    return 0;
}

int removeFdFromReactor(void* reactorPtr, int fd) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    FD_CLR(fd, &reactor->masterSet); // Remove the file descriptor from the master set
    reactor->handlers.erase(fd); // Erase the handler associated with the file descriptor
    if (fd == reactor->fdMax) {
        // Update the maximum file descriptor value
        while (!FD_ISSET(reactor->fdMax, &reactor->masterSet) && reactor->fdMax > 0) {
            --reactor->fdMax;
        }
    }
    return 0;
}

int stopReactor(void* reactorPtr) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    reactor->running = false; // Set running flag to false to stop the loop
    return 0;
}

void reactorLoop(void* reactorPtr) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    while (reactor->running) {
        reactor->readSet = reactor->masterSet; // Copy master set to read set
        int activity = select(reactor->fdMax + 1, &reactor->readSet, nullptr, nullptr, nullptr); // Wait for activity on any file descriptor
        if (activity < 0 && errno != EINTR) {
            std::cerr << "Error on select" << std::endl;
            break;
        }
        for (int i = 0; i <= reactor->fdMax; ++i) {
            if (FD_ISSET(i, &reactor->readSet)) { // Check if the file descriptor is ready for reading
                reactor->handlers[i](i); // Call the handler associated with the file descriptor
            }
        }
    }
    delete reactor; // Clean up reactor resources
}

// Proactor functions

void* proactorHandler(void* arg) {
    ProactorArg* proactorArg = static_cast<ProactorArg*>(arg);
    int clientSocket = proactorArg->sockfd;
    proactorFunc handler = proactorArg->handler;
    delete proactorArg; // Clean up the argument structure

    handler(clientSocket); // Call the handler function with the client socket

    return nullptr;
}

void* proactorWrapper(void* arg) {
    return proactorHandler(arg); // Wrapper to call proactorHandler
}

pthread_t startProactor(int sockfd, proactorFunc threadFunc) {
    pthread_t tid;
    ProactorArg* proactorArg = new ProactorArg{sockfd, threadFunc};
    if (pthread_create(&tid, nullptr, proactorWrapper, proactorArg) != 0) {
        cerr << "Error creating proactor thread" << endl;
        delete proactorArg; // Clean up on failure
    }
    return tid;
}

int stopProactor(pthread_t tid) {
    return pthread_cancel(tid); // Cancel the proactor thread
}
