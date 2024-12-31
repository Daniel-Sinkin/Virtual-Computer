cmake_minimum_required(VERSION 3.10)

project(MainProject)

set(CMAKE_C_STANDARD 99)

file(GLOB SRC_FILES src/*.c)
set(SOURCES main.c ${SRC_FILES})

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(MainProject ${SOURCES})