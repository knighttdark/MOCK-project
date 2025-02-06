# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MusicPlayer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags for compiling with coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

# Include directories
include_directories(
    /usr/local/include/taglib
    /usr/local/include
    ${CMAKE_SOURCE_DIR}/header
    ${CMAKE_SOURCE_DIR}/header/include
    ${CMAKE_SOURCE_DIR}/header/model
    ${CMAKE_SOURCE_DIR}/header/controller
    ${CMAKE_SOURCE_DIR}/header/view
)

# Link directories
link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

# Libraries
set(LIBRARIES
    avformat avcodec avutil swresample swscale
    tag SDL2 SDL2_mixer SDL2_ttf SDL2_image stdc++fs z m pthread
)

# Directories for source files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/source)
set(MODEL_DIR ${SRC_DIR}/model)
set(CONTROLLER_DIR ${SRC_DIR}/controller)
set(VIEW_DIR ${SRC_DIR}/view)
set(INCLUDE_DIR ${SRC_DIR}/include)

# Find all source files
file(GLOB MODEL_SOURCES ${MODEL_DIR}/*.cpp)
file(GLOB CONTROLLER_SOURCES ${CONTROLLER_DIR}/*.cpp)
file(GLOB VIEW_SOURCES ${VIEW_DIR}/*.cpp)
file(GLOB INCLUDE_SOURCES ${INCLUDE_DIR}/*.cpp)
set(MAIN_SOURCE ${SRC_DIR}/main.cpp)

# Add the sources
set(SOURCES ${MODEL_SOURCES} ${CONTROLLER_SOURCES} ${VIEW_SOURCES} ${INCLUDE_SOURCES} ${MAIN_SOURCE})

# Output object directory
set(OBJ_DIR ${CMAKE_BINARY_DIR}/obj)
file(MAKE_DIRECTORY ${OBJ_DIR})

# Main application
add_executable(MusicPlayer ${SOURCES})
target_link_libraries(MusicPlayer ${LIBRARIES})

# -------------------------------
# Setup for unit testing with GTest & GMock
# -------------------------------

# Enable testing
enable_testing()

# Find Google Test and Google Mock
find_package(GTest REQUIRED)
find_library(GMOCK_LIB gmock HINTS /usr/local/lib)
find_library(GMOCK_MAIN_LIB gmock_main HINTS /usr/local/lib)

# Ensure that GMock is found
if(GMOCK_LIB AND GMOCK_MAIN_LIB)
    message(STATUS "Found GMock: ${GMOCK_LIB}, ${GMOCK_MAIN_LIB}")
    set(GMOCK_LIBRARIES ${GMOCK_LIB} ${GMOCK_MAIN_LIB})
else()
    message(FATAL_ERROR "GMock library not found!")
endif()

# Include directories for GTest and GMock
include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include/gmock)

# Find all test source files
file(GLOB TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/*.cpp)

# Add the test library
add_library(controller_lib ${CONTROLLER_SOURCES} ${MODEL_SOURCES} ${VIEW_SOURCES} ${INCLUDE_SOURCES})
target_compile_options(controller_lib PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(controller_lib gcov)

# Create the test executable
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests 
    ${GTEST_BOTH_LIBRARIES} 
    ${GMOCK_LIBRARIES} 
    GTest::GTest 
    GTest::Main 
    pthread 
    ${LIBRARIES} 
    controller_lib
)

# Add test target
add_test(NAME UnitTests COMMAND unit_tests)

# -------------------------------
# Custom Targets for Cleaning and Coverage
# -------------------------------

# Clean all target (removes build directories)
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}  # Remove build directory
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/coverage_report  # Remove coverage directory
    COMMENT "Cleaning build and coverage report directories"
)

# Create a directory for coverage reports
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/coverage_report)

# Coverage report target
add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/coverage_report
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/source/controller --html --html-details -o ${CMAKE_SOURCE_DIR}/coverage_report/index.html
    COMMENT "Generating coverage report for source/controller"
)

# Default build target that depends on unit tests and main executable
add_custom_target(build_all
    DEPENDS unit_tests MusicPlayer
)

