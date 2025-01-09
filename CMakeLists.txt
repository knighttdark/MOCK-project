cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MOCK-project VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
include_directories(src/hardware)

# Add source files
file(GLOB SOURCES "src/*.cpp" "src/hardware/*.cpp")

# Create the executable
add_executable(media_player ${SOURCES})

# Link with external libraries (e.g., Taglib)
target_link_libraries(media_player /usr/local/lib/libtag.a)
