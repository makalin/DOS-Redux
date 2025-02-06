cmake_minimum_required(VERSION 3.10)
project(DOS_Redux C ASM)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src/bootloader)
add_subdirectory(src/kernel)
add_subdirectory(src/shell)

# Set build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Build targets
add_executable(dosredux
    src/kernel/kernel.c
    src/shell/shell.c
) 