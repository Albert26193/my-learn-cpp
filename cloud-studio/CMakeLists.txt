# Specify minimum required CMake version
cmake_minimum_required(VERSION 3.13)

# Check C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project name
project(cloud-studio-exercises)

# Set Google Test root directory path
set(GTest_ROOT ${CMAKE_SOURCE_DIR}/tools/gtest)

# Enable generation of compile_commands.json for clangd (language server)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Google Test package
# Question: How can we find the Google Test package?
# GTEST_ROOT --> find GTEST -> include GTEST_INCLUDE_DIRS -> link GTEST_BOTH_LIBRARIES
find_package(GTest REQUIRED)

# Include Google Test header directories
include_directories(${GTEST_INCLUDE_DIRS})

# Collect all source files from exercises directories
file(GLOB_RECURSE SRC_EXERCISES ${CMAKE_CURRENT_SOURCE_DIR}/chapters/**/exercises/*.cpp)

# Collect all test files from tests directories
file(GLOB_RECURSE SRC_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/chapters/**/tests/*.cpp)

# Include all exercise directories to make their headers available
# Include all exercise directories to make their headers available
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/chapters/c1/exercises
    ${CMAKE_CURRENT_SOURCE_DIR}/chapters/c1
    ${CMAKE_CURRENT_SOURCE_DIR}/chapters
)

# Define the executable target for unit tests
add_executable(unit_test ${SRC_EXERCISES} ${SRC_TESTS})

# Link Google Test libraries to the unit test executable
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})