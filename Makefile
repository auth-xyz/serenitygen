# Variables
SRC_DIR = ./src
BIN_DIR = ./dist
BUILD_DIR = ./build
EXECUTABLE = serenity 

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wno-unused-parameter -DDPP_CORO
LDFLAGS = -ldpp -lpthread

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RED = \033[0;31m
NC = \033[0m # No Color

# Find all .cpp files
CPP_FILES = $(shell find $(SRC_DIR) -name "*.cpp")

# Generate .o files from .cpp files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))

# Build target
all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	@echo -e "$(YELLOW)[link] Linking object files to create the executable$(NC)"
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)
	@if [ $$? -eq 0 ]; then \
		echo -e "$(GREEN)[link] Build successful! Executable created at $(BIN_DIR)/$(EXECUTABLE)$(NC)"; \
	else \
		echo -e "$(RED)[link] Build failed!$(NC)"; \
	fi

# Rule to compile .cpp files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo -e "$(YELLOW)[gcc] Compiling $< to $@$(NC)"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@if [ $$? -eq 0 ]; then \
		echo -e "$(YELLOW)[gcc] Finished building binary $@$(NC)"; \
	else \
		echo -e "$(RED)[gcc] Compilation failed for $<$(NC)"; \
	fi

clean:
	@echo -e "$(YELLOW)[clean] Removing binaries and build files$(NC)"
	@rm -rf $(BIN_DIR)/$(EXECUTABLE) $(BUILD_DIR)

.PHONY: all clean

