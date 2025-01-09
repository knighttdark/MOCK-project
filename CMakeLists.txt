# CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(MusicPlayer CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Include directories
include_directories(
    /usr/local/include
    /usr/include
    /usr/local/include/taglib
    ${CMAKE_SOURCE_DIR}/include
)

# Linker flags
set(LINKER_FLAGS "-L/usr/local/lib -L/usr/lib -ltag -lz")

# Add source files
file(GLOB_RECURSE SOURCES "src/**/*.cpp")

# Add object files
set(OBJECTS)
foreach(SRC ${SOURCES})
    get_filename_component(OBJ ${SRC} NAME_WE)
    list(APPEND OBJECTS "${CMAKE_BINARY_DIR}/build/${OBJ}.o")
endforeach()

# Add executable
add_executable(MusicPlayer ${OBJECTS})

# Link libraries
target_link_libraries(MusicPlayer ${LINKER_FLAGS})

# Compilation settings for individual object files
foreach(SRC ${SOURCES})
    get_filename_component(OBJ ${SRC} NAME_WE)
    set(OBJ_DIR ${CMAKE_BINARY_DIR}/build/${OBJ}.o)
    
    add_custom_command(
        OUTPUT ${OBJ_DIR}
        COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -I${CMAKE_SOURCE_DIR}/include -c ${SRC} -o ${OBJ_DIR}
        DEPENDS ${SRC}
        COMMENT "Compiling ${SRC} to ${OBJ_DIR}"
    )
endforeach()

# Clean up the build (equivalent to "make clean")
add_custom_target(clean_project
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/build ${CMAKE_BINARY_DIR}/bin/*
    COMMENT "Cleaning the build"
)
