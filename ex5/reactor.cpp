#include "reactor.hpp"
#include <unistd.h>
#include <iostream>

void* startReactor() {
    Reactor* reactor = new Reactor();
    FD_ZERO(&reactor->masterSet); // Initialize the master file descriptor set
    FD_ZERO(&reactor->readSet); // Initialize the read file descriptor set
    reactor->fdMax = 0; // Set the initial maximum file descriptor value to 0
    reactor->running = true; // Set the reactor running flag to true
    return reactor; // Return the initialized reactor
}

int addFdToReactor(void* reactorPtr, int fd, reactorFunc func) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    FD_SET(fd, &reactor->masterSet); // Add the file descriptor to the master set
    if (fd > reactor->fdMax) { // Update the maximum file descriptor value if necessary
        reactor->fdMax = fd;
    }
    reactor->handlers[fd] = func; // Associate the handler function with the file descriptor
    return 0; // Return 0 to indicate success
}

int removeFdFromReactor(void* reactorPtr, int fd) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    FD_CLR(fd, &reactor->masterSet); // Remove the file descriptor from the master set
    reactor->handlers.erase(fd); // Remove the handler function associated with the file descriptor
    if (fd == reactor->fdMax) { // Adjust the maximum file descriptor value if necessary
        while (!FD_ISSET(reactor->fdMax, &reactor->masterSet) && reactor->fdMax > 0) {
            --reactor->fdMax; // Decrement the maximum file descriptor value until a valid one is found
        }
    }
    return 0; // Return 0 to indicate success
}

int stopReactor(void* reactorPtr) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    reactor->running = false; // Set the reactor running flag to false to stop the loop
    return 0; // Return 0 to indicate success
}

void reactorLoop(void* reactorPtr) {
    Reactor* reactor = static_cast<Reactor*>(reactorPtr);
    while (reactor->running) { // Continue looping while the reactor is running
        reactor->readSet = reactor->masterSet; // Copy the master set to the read set for select
        int activity = select(reactor->fdMax + 1, &reactor->readSet, nullptr, nullptr, nullptr); // Wait for activity on any file descriptor
        if (activity < 0 && errno != EINTR) { // Check for errors in select call
            std::cerr << "Error on select" << std::endl;
            break; // Break the loop if there is an error
        }
        for (int i = 0; i <= reactor->fdMax; ++i) { // Iterate over all possible file descriptors
            if (FD_ISSET(i, &reactor->readSet)) { // Check if the file descriptor is ready for reading
                reactor->handlers[i](i); // Call the handler function associated with the file descriptor
            }
        }
    }
    delete reactor; // Clean up and delete the reactor
}