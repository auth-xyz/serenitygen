# Variables
SRC_DIR = ./src
BIN_DIR = ./dist
BUILD_DIR = ./build
EXECUTABLE = discord_bot

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -DDPP_CORO
LDFLAGS = -ldpp -lpthread

# Find all .cpp files
CPP_FILES = $(shell find $(SRC_DIR) -name "*.cpp")

# Generate .o files from .cpp files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))

# Build target
all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)
	@if [ $$? -eq 0 ]; then \
		echo "Build successful! Executable created at $(BIN_DIR)/$(EXECUTABLE)"; \
	else \
		echo "Build failed!"; \
	fi

# Rule to compile .cpp files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/$(EXECUTABLE) $(BUILD_DIR)

.PHONY: all clean

