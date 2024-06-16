#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <map>
#include <functional>
#include <sys/select.h>
#include <pthread.h>

// Define the reactor function type
typedef std::function<void(int)> reactorFunc;

// Define the reactor structure
struct Reactor {
    fd_set masterSet;
    fd_set readSet;
    int fdMax;
    std::map<int, reactorFunc> handlers;
    bool running;
};

// Function prototypes for reactor
void* startReactor();
int addFdToReactor(void* reactor, int fd, reactorFunc func);
int removeFdFromReactor(void* reactor, int fd);
int stopReactor(void* reactor);
void reactorLoop(void* reactorPtr);

// Define the proactor function type
typedef void* (*proactorFunc)(int sockfd);

// Struct to hold the argument for proactorWrapper
struct ProactorArg {
    int sockfd;
    proactorFunc handler;
};

// Function prototypes for proactor
pthread_t startProactor(int sockfd, proactorFunc threadFunc);
int stopProactor(pthread_t tid);

// Wrapper function for proactor
void* proactorWrapper(void* arg);

#endif // REACTOR_HPP
