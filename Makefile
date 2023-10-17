# Makefile for compiling C++ code on Windows

# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11
# Source files directory
SRCDIR = src
# Output directory
BINDIR = bin

# List of source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# List of object files (same names as source files, but with .o extension)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
# Name of the output executable
EXECUTABLE = $(BINDIR)/out

# Default target
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Rule to compile source files into object files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean the compiled files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
