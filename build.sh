#!/bin/bash

# Variables
SRC_DIR="./src"
BIN_DIR="./dist"
EXECUTABLE="discord_bot"

# Create build directories if they don't exist
mkdir -p $BIN_DIR

# Find all .cpp files
CPP_FILES=$(find $SRC_DIR -name "*.cpp")

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra"
LDFLAGS="-ldpp -lpthread" # Link against dpp and pthread libraries

# Compile and link
$CXX $CXXFLAGS $CPP_FILES -o $BIN_DIR/$EXECUTABLE $LDFLAGS

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Executable created at $BIN_DIR/$EXECUTABLE"
else
    echo "Build failed!"
fi

