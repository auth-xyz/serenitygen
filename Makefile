# Variables
SRC_DIR = ./src
BIN_DIR = ./dist
EXECUTABLE = discord_bot

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter
LDFLAGS = -ldpp -lpthread

# Find all .cpp files
CPP_FILES = $(shell find $(SRC_DIR) -name "*.cpp")

# Build target
all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(CPP_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CPP_FILES) -o $@ $(LDFLAGS)
	@if [ $$? -eq 0 ]; then \
		echo "Build successful! Executable created at $(BIN_DIR)/$(EXECUTABLE)"; \
	else \
		echo "Build failed!"; \
	fi

clean:
	rm -f $(BIN_DIR)/$(EXECUTABLE)

.PHONY: all clean

