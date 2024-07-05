cmake_minimum_required(VERSION 3.10)
project(discord_bot_project CXX)

# Set variables
set(SRC_DIR "./src")
set(BIN_DIR "./dist")
set(EXECUTABLE "discord_bot")

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find all .cpp files
file(GLOB_RECURSE CPP_FILES "${SRC_DIR}/*.cpp")

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Add pthread and dpp libraries for linking (assuming they are found by MinGW)
set(LINK_LIBRARIES pthread dpp)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# Build executable
add_executable(${EXECUTABLE} ${CPP_FILES})
target_link_libraries(${EXECUTABLE} ${LINK_LIBRARIES})

