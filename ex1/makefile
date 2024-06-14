# Variables
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = kosaraju
SRC = kosaraju.cpp

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean target
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
