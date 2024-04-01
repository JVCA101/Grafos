# Compiler
CXX := g++

# Directories
SRC_DIR := source
INC_DIR := include

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS := $(patsubst $(SRC_DIR)/%.cpp,%.o,$(filter-out main.cpp,$(SRCS)))
MAIN_OBJ := main.o
DEPS := $(wildcard $(INC_DIR)/*.hpp)

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Output executable
TARGET := graph_project

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(MAIN_OBJ): main.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	@rm -f $(OBJS) $(MAIN_OBJ) $(TARGET)
