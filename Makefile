# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -Wall

# Project settings
PROJECT_NAME = backend
SRC_DIR = src
BUILD_DIR = build
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(PROJECT_NAME)

# Linking
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
