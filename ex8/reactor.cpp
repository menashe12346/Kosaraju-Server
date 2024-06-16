#include "reactor.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring> // For memset

using namespace std;

void* startReactor() {
    Reactor* reactor = new Reactor();
    FD_ZERO(&reactor->masterSet);
    FD_ZERO(&reactor->readSet);
    reactor->fdMax = 0;
    reactor->running = true;
    return reactor;
}

int addFdToReactor(void* reactorPtr, int fd, reactorFunc func) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    FD_SET(fd, &reactor->masterSet);
    if (fd > reactor->fdMax) {
        reactor->fdMax = fd;
    }
    reactor->handlers[fd] = func;
    return 0;
}

int removeFdFromReactor(void* reactorPtr, int fd) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    FD_CLR(fd, &reactor->masterSet);
    reactor->handlers.erase(fd);
    if (fd == reactor->fdMax) {
        while (!FD_ISSET(reactor->fdMax, &reactor->masterSet) && reactor->fdMax > 0) {
            --reactor->fdMax;
        }
    }
    return 0;
}

int stopReactor(void* reactorPtr) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    reactor->running = false;
    return 0;
}

void reactorLoop(void* reactorPtr) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    while (reactor->running) {
        reactor->readSet = reactor->masterSet;
        int activity = select(reactor->fdMax + 1, &reactor->readSet, nullptr, nullptr, nullptr);
        if (activity < 0 && errno != EINTR) {
            std::cerr << "Error on select" << std::endl;
            break;
        }
        for (int i = 0; i <= reactor->fdMax; ++i) {
            if (FD_ISSET(i, &reactor->readSet)) {
                reactor->handlers[i](i);
            }
        }
    }
    delete reactor;
}

// Proactor functions

void* proactorHandler(void* arg) {
    ProactorArg* proactorArg = static_cast<ProactorArg*>(arg);
    int clientSocket = proactorArg->sockfd;
    proactorFunc handler = proactorArg->handler;
    delete proactorArg;

    handler(clientSocket);

    return nullptr;
}

void* proactorWrapper(void* arg) {
    return proactorHandler(arg);
}

pthread_t startProactor(int sockfd, proactorFunc threadFunc) {
    pthread_t tid;
    ProactorArg* proactorArg = new ProactorArg{sockfd, threadFunc};
    if (pthread_create(&tid, nullptr, proactorWrapper, proactorArg) != 0) {
        cerr << "Error creating proactor thread" << endl;
        delete proactorArg;
    }
    return tid;
}

int stopProactor(pthread_t tid) {
    return pthread_cancel(tid);
}
