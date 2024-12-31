cmake_minimum_required(VERSION 3.10)

project(MainProject)

set(CMAKE_C_STANDARD 99)

# Main project sources
file(GLOB SRC_FILES src/*.c)
set(SOURCES main.c ${SRC_FILES})

# Include directories for the main project
include_directories(${CMAKE_SOURCE_DIR}/src)

# Main executable
add_executable(MainProject ${SOURCES})

# Add Unity as a testing framework
enable_testing()

# Test sources
file(GLOB TEST_FILES tests/*.c)
include_directories(${CMAKE_SOURCE_DIR}/tests)

# Add the Unity testing framework
set(UNITY_SRC
    ${CMAKE_SOURCE_DIR}/external/unity/unity.c
)
include_directories(${CMAKE_SOURCE_DIR}/external/unity)

# Create a test executable
add_executable(Tests ${TEST_FILES} ${UNITY_SRC} ${SRC_FILES})

# Add tests to CTest
add_test(NAME UnitTests COMMAND Tests)