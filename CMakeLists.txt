# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and settings
project(MediaPlayer VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Directories
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(BUILD_DIR build)
set(BIN_DIR bin)

# Sources and headers
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
include_directories(${INCLUDE_DIR})

# Output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR})

# Set the prefix path to help CMake find SDL2_mixer
# Adjust this path based on the location of SDL2_mixerConfig.cmake on your system
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer")

# Find SDL2 and SDL2_mixer
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    SDL2::SDL2_mixer
)

# Output project configuration
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Sources: ${SOURCES}")
message(STATUS "Include Directory: ${INCLUDE_DIR}")
message(STATUS "Output Binary Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
