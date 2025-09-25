# Makefile for SIMLIB-based simulation project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lsimlib -lstdc++

# Source files and executable
SRC = src/main.cpp src/customer_generator.cpp src/work_shift.cpp src/customer.cpp 
EXEC = main

# Default target
all: $(EXEC)

# Build the executable
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -f $(EXEC) *.o

# Phony targets
.PHONY: all clean
