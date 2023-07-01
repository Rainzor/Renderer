# Variables
CXX = g++
CXXFLAGS = -std=c++17 -I../include -I../include/common -I../include/shapes -I../include/external -fopenmp
LDFLAGS = -fopenmp
SOURCES = $(wildcard *.cpp) $(wildcard ../src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = example.exe

# Main target
all: $(EXECUTABLE)

# Linking the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
.PHONY: clean
clean:
	del /Q /F $(subst /,\,$(OBJECTS)) $(EXECUTABLE)