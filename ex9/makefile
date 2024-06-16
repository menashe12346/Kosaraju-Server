# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

# Executable names
SERVER_TARGET = server
CLIENT_TARGET = client

# Source files
SERVER_SRCS = server.cpp ../ex3/kosaraju_vector_list.cpp ../ex8/reactor.cpp
CLIENT_SRCS = client.cpp

# Object files
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

# Default target
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Link the server executable
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJS) -lpthread

# Link the client executable
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJS)

# Compile the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(SERVER_OBJS) $(SERVER_TARGET) $(CLIENT_OBJS) $(CLIENT_TARGET)

# Phony targets
.PHONY: all clean
