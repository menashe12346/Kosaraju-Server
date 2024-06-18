#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <map>
#include <functional>
#include <sys/select.h>
#include <pthread.h>

/* 
The Reactor Pattern allows for efficient management of multiple I/O sources without the need for multi-threading.
Instead, it uses asynchronous I/O to notify the application when it is possible to read or write data.

the Proactor pattern allows for the event handling to be initiated by the completion of asynchronous operations.
This means that the system can perform other tasks while waiting for the completion of I/O operations
*/

/*
Using a Proactor with a Reactor combines the benefits of both patterns:

1. Reactor: Handles synchronous events efficiently. It's great for managing multiple connections and events using select(), poll(), or similar mechanisms.
2. Proactor: Handles asynchronous events, allowing other tasks to proceed while waiting for I/O operations to complete.
*/

// Define the reactor function type
/// @brief Type definition for reactor function.
/// @param int File descriptor associated with the function.
typedef std::function<void(int)> reactorFunc;

// Define the reactor structure
/// @brief Structure to hold reactor information and manage event-driven programming.
struct Reactor {
    fd_set masterSet; ///< Master set of file descriptors to monitor.
    fd_set readSet;   ///< Temporary set for select().
    int fdMax;        ///< Maximum file descriptor value.
    std::map<int, reactorFunc> handlers; ///< Map of file descriptors to their associated handlers.
    bool running;     ///< Flag to indicate if the reactor is running.
};

// Function prototypes for reactor

/// @brief Starts a new reactor.
/// @return Pointer to the created reactor.
void* startReactor();

/// @brief Adds a file descriptor to the reactor.
/// @param reactor Pointer to the reactor.
/// @param fd File descriptor to add.
/// @param func Function to handle events on the file descriptor.
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

/// @brief Main loop for the reactor to handle events.
/// @param reactorPtr Pointer to the reactor.
void reactorLoop(void* reactorPtr);

// Define the proactor function type
/// @brief Type definition for proactor function.
/// @param int File descriptor associated with the function.
/// @return Pointer to the result.
typedef void* (*proactorFunc)(int sockfd);

// Struct to hold the argument for proactorWrapper
/// @brief Structure to hold arguments for the proactor wrapper function.
struct ProactorArg {
    int sockfd;            ///< File descriptor for the socket.
    proactorFunc handler;  ///< Handler function to process the socket events.
};

// Function prototypes for proactor

/**
 * @brief Starts a new proactor thread to handle a client connection.
 * 
 * This function creates a new thread to handle the client connection using a
 * proactor pattern. It wraps the socket file descriptor and the handling function
 * into a structure that is passed to the new thread.
 * 
 * @param sockfd The socket file descriptor of the client connection.
 * @param threadFunc The function to handle the client connection.
 * @return pthread_t The ID of the newly created thread.
 */
pthread_t startProactor(int sockfd, proactorFunc threadFunc);

/// @brief Stops a proactor thread.
/// @param tid Thread ID of the proactor thread to stop.
/// @return 0 on success, -1 on failure.
int stopProactor(pthread_t tid);

/// @brief Wrapper function for proactor to handle thread arguments.
/// @param arg Pointer to the ProactorArg structure.
/// @return Pointer to the result.
void* proactorWrapper(void* arg);

#endif // REACTOR_HPP
