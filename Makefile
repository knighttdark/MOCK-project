# # Compiler settings
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -g -I/usr/local/include -I/usr/include -I/usr/local/include/SDL2 -Iinclude

# LDFLAGS = -L/usr/local/lib -L/usr/lib -L/lib/x86_64-linux-gnu \
#     -lSDL2 -lSDL2_mixer -lSDL2_ttf -ltag -lz \
#     -lavcodec -lavformat -lavutil -lswscale \
#     -lftxui-component -lftxui-dom -lftxui-screen \
#     -pthread -ldl 

# # Directories
# SRC_DIR = src
# OBJ_DIR = build
# BIN_DIR = bin

# # Sources and objects
# SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
# OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.cpp=.o))
# EXEC = $(BIN_DIR)/MusicPlayer

# # Targets
# all: $(EXEC)

# # Link the object files into the final executable
# $(EXEC): $(OBJECTS)
# 	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
# 	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# # Compile the source files into object files
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean up the build
# clean:
# 	rm -rf $(OBJ_DIR) $(BIN_DIR)/*

# .PHONY: all clean
# Compiler settings
# Compiler settings
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I/usr/local/include -I/usr/include -I/usr/local/include/SDL2 -Iinclude -fprofile-arcs -ftest-coverage
LDFLAGS = -L/usr/local/lib -L/usr/lib -L/lib/x86_64-linux-gnu \
    -lSDL2 -lSDL2_mixer -lSDL2_ttf -ltag -lz \
    -lavcodec -lavformat -lavutil -lswscale \
    -lftxui-component -lftxui-dom -lftxui-screen \
    -pthread -ldl -lgtest -lgtest_main -fprofile-arcs -ftest-coverage

# Directories
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = build/objects
BIN_DIR = bin

# Sources
MAIN_SOURCE = $(SRC_DIR)/main.cpp
SOURCES := $(filter-out $(MAIN_SOURCE), $(shell find $(SRC_DIR) -type f -name "*.cpp"))
TEST_SOURCES := $(shell find $(TEST_DIR) -type f -name "*.cpp")

# Objects
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/src/%.o, $(SOURCES))
MAIN_OBJECT := $(OBJ_DIR)/src/main.o
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/test/%.o, $(TEST_SOURCES))

# Executables
EXEC = $(BIN_DIR)/MusicPlayer
TEST_EXEC = $(BIN_DIR)/test_runner

# Default target
all: $(EXEC)

# Build the main application
$(EXEC): $(OBJECTS) $(MAIN_OBJECT)
	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CXX) $(OBJECTS) $(MAIN_OBJECT) -o $@ $(LDFLAGS)

# Build the main.cpp file
$(MAIN_OBJECT): $(MAIN_SOURCE)
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run tests
test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Build the test executable
$(TEST_EXEC): $(OBJECTS) $(TEST_OBJECTS)
	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CXX) $(TEST_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files into object files
$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean_src_o:
	find $(SRC_DIR) -type f -name "*.o" -delete
	find $(TEST_DIR) -type f -name "*.o" -delete
# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean test
