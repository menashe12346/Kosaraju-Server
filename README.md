
# 🖥️ Operating Systems Exercise 3: Synchronization and Kosaraju Server

This repository contains the implementation of the **Kosaraju-Sharir algorithm** for finding strongly connected components (SCC) in a graph. The project includes several stages, each building on the previous one to integrate synchronization, server capabilities, and various design patterns.

## 🌟 Overview

The project aims to develop a robust server capable of managing graph operations and computing strongly connected components (SCCs) using the Kosaraju-Sharir algorithm. It is designed to efficiently handle multiple clients simultaneously, ensuring seamless interaction and synchronization.

## 🚀 Stages of Implementation

### Stage 1: Implementing Kosaraju's Algorithm
The first stage involves implementing the **Kosaraju-Sharir algorithm** to find SCCs in a directed graph. The server reads the graph's vertices and edges, processes them, and outputs the SCCs. This forms the core functionality of the project.

### Stage 2: Profiling Different Implementations
In this stage, the project focuses on profiling different implementations of lists used in the algorithm. By comparing various list implementations, such as `std::list` and `std::deque` in C++, we determine the most efficient one for our use case.

### Stage 3: Client Interaction via Stdin
Here, the server is extended to support interaction with clients via stdin. Clients can issue commands to create new graphs, add or remove edges, and run the Kosaraju algorithm. This stage enhances the server's usability and interactivity.

### Stage 4: Supporting Multiple Users
The server is further enhanced to support multiple users. Each user can independently interact with the shared graph, issuing commands and receiving results. Synchronization mechanisms ensure that the shared state remains consistent across all users.

### Stage 5: Implementing the Reactor Pattern
The reactor pattern is implemented to handle multiple client requests efficiently. This design pattern allows the server to handle I/O operations asynchronously, improving performance and scalability.

### Stage 6: Multi-threaded Implementation
The server is modified to handle multiple clients using threads. Each client connection is managed by a separate thread, ensuring concurrent processing of requests. Synchronization mechanisms are employed to handle shared resources safely.

### Stage 7: Implementing the Proactor Pattern
Building on the previous stages, the server is re-implemented using the proactor pattern. This pattern further enhances the server's ability to handle asynchronous I/O operations, improving its responsiveness and efficiency.

### Stage 8: Combining Reactor and Proactor Patterns
The server combines the reactor and proactor patterns to optimize handling client requests and processing graph operations. This hybrid approach leverages the strengths of both patterns for maximum performance.

### Stage 9: Producer-Consumer Thread Model
Finally, a producer-consumer thread model is added to the server. An additional thread monitors the graph's state and prints a message when at least 50% of the vertices are in a single SCC. This stage ensures that the server can handle complex, real-time processing requirements.

## ⚙️ Setup

To set up and run these exercises, follow the instructions below:

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd operating_systems_ex3
   ```

2. **Compile the code:**
   Each stage may have its own compilation instructions. Generally, you can use `make` to compile the code in each stage's directory.

3. **Run the server:**
   Refer to the specific instructions provided in each stage's directory to start the server and interact with it.