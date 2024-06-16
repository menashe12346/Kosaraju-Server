#ifndef REACTOR_H
#define REACTOR_H

#include <map>
#include <functional>
#include <sys/select.h>

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

// Function prototypes
void* startReactor();
int addFdToReactor(void* reactor, int fd, reactorFunc func);
int removeFdFromReactor(void* reactor, int fd);
int stopReactor(void* reactor);
void reactorLoop(void* reactorPtr); // Add this line

#endif // REACTOR_H
