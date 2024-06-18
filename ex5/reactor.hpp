#ifndef REACTOR_H
#define REACTOR_H

#include <map>
#include <functional>
#include <sys/select.h>

/* 
The Reactor Pattern allows for efficient management of multiple I/O sources without the need for multi-threading.
Instead, it uses asynchronous I/O to notify the application when it is possible to read or write data.
*/

// Define the reactor function type
/// @brief Type definition for reactor functions.
/// A reactor function takes an integer file descriptor as its parameter.
typedef std::function<void(int)> reactorFunc;

// Define the reactor structure
/// @brief Structure representing the reactor.
/// Contains file descriptor sets, a maximum file descriptor value, a map of handlers, and a running flag.
struct Reactor {
    fd_set masterSet; ///< Master file descriptor set for all descriptors.
    fd_set readSet; ///< Temporary file descriptor set for reading.
    int fdMax; ///< Maximum file descriptor value.
    std::map<int, reactorFunc> handlers; ///< Map of file descriptors to their corresponding handler functions.
    bool running; ///< Flag indicating if the reactor is running.
};

// Function prototypes

/// @brief Starts the reactor.
/// @return A pointer to the newly created reactor.
void* startReactor();

/// @brief Adds a file descriptor and its handler function to the reactor.
/// @param reactor Pointer to the reactor.
/// @param fd File descriptor to add.
/// @param func Handler function to associate with the file descriptor.
/// @return 0 on success, -1 on failure.
int addFdToReactor(void* reactor, int fd, reactorFunc func);

/// @brief Removes a file descriptor from the reactor.
/// @param reactor Pointer to the reactor.
/// @param fd File descriptor to remove.
/// @return 0 on success, -1 on failure.
int removeFdFromReactor(void* reactor, int fd);

/// @brief Stops the reactor.
/// @param reactor Pointer to the reactor.
/// @return 0 on success, -1 on failure.
int stopReactor(void* reactor);

/// @brief Main loop for the reactor.
/// @param reactorPtr Pointer to the reactor.
void reactorLoop(void* reactorPtr);

#endif // REACTOR_H
