# # Compiler settings
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -I/usr/local/include -I/usr/include -I/usr/local/include/taglib

# LDFLAGS = -L/usr/local/lib -L/usr/lib -ltag -lz  # Thêm -lz để liên kết với zlib

# # Directories
# SRC_DIR = src
# OBJ_DIR = build
# BIN_DIR = bin
# INCLUDE_DIR = include

# # Sources and objects
# SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
# OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.cpp=.o))
# EXEC = $(BIN_DIR)/MusicPlayer

# # Targets
# all: $(EXEC)

# # Link the object files into the final executable
# $(EXEC): $(OBJECTS)
# 	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
# 	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# # Compile the source files into object files
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
# 	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# # Clean up the build
# clean:
# 	rm -rf $(OBJ_DIR) $(BIN_DIR)/*

# .PHONY: all clean
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I/usr/local/include -I/usr/include -I/usr/local/include/taglib

LDFLAGS = -L/usr/local/lib -L/usr/lib -ltag -lz  # Thêm -lz để liên kết với zlib

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Sources and objects
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.cpp=.o))
EXEC = $(BIN_DIR)/MusicPlayer

# Targets
all: $(EXEC)

# Link the object files into the final executable
$(EXEC): $(OBJECTS)
	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Compile the source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean up the build
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/*

.PHONY: all clean

