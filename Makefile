CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I/usr/local/include -I/usr/include -I/usr/local/include/SDL2 -Iinclude -fprofile-arcs -ftest-coverage #-fsanitize=address -g -O0
LDFLAGS = -L/usr/local/lib -L/usr/lib -L/lib/x86_64-linux-gnu \
    -lSDL2 -lSDL2_mixer -lSDL2_ttf -ltag -lz \
    -lavcodec -lavformat -lavutil -lswscale \
    -lftxui-component -lftxui-dom -lftxui-screen \
    -pthread -ldl -lgtest -lgtest_main -lgmock -lgmock_main -fprofile-arcs -ftest-coverage  #-fsanitize=address

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
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) $(MAIN_OBJECT) -o $@ $(LDFLAGS)

# Build the main.cpp file
$(MAIN_OBJECT): $(MAIN_SOURCE)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run the program
run: $(EXEC)
	@echo "Running the program..."
	@./$(EXEC)

# Build and run tests
test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Build the test executable
$(TEST_EXEC): $(OBJECTS) $(TEST_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(TEST_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files into object files
$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Generate gcov reports
GCOV_DIR = coverage
gcov:
	@mkdir -p $(GCOV_DIR)
	find $(OBJ_DIR) -name "*.gcno" | while read gcno_file; do \
		src_file=$$(echo $$gcno_file | sed "s|$(OBJ_DIR)/src|$(SRC_DIR)|;s|\.gcno|.cpp|"); \
		if [ -f $$src_file ]; then \
			echo "Processing: $$src_file"; \
			gcov -o $$(dirname $$gcno_file) $$src_file > /dev/null; \
		fi; \
	done
	find . -name "*.gcov" -exec mv {} $(GCOV_DIR) \;
	find $(GCOV_DIR) -type f -name "*.gcov" | grep -v "\.cpp\.gcov" | xargs rm -f
	@echo "Coverage Summary:"
	@grep "Lines executed" $(GCOV_DIR)/*.cpp.gcov || echo "No coverage information found."

# Generate lcov reports
LCOV_DIR = coverage-lcov
LCOV_INFO = $(LCOV_DIR)/coverage.info
HTML_OUTPUT = $(LCOV_DIR)/html

# lcov:
# 	@mkdir -p $(LCOV_DIR)
# 	@echo "Capturing coverage data..."
# 	lcov --capture --directory $(OBJ_DIR) --output-file $(LCOV_INFO) --rc lcov_branch_coverage=1
# 	@echo "Filtering out system and test files..."
# 	lcov --remove $(LCOV_INFO) "/usr/*" "*/test/*" --output-file $(LCOV_INFO) --rc lcov_branch_coverage=1
# 	@echo "Generating HTML report..."
# 	genhtml $(LCOV_INFO) --output-directory $(HTML_OUTPUT) --rc lcov_branch_coverage=1
# 	@echo "HTML report generated at $(HTML_OUTPUT)/index.html"

lcov:
	@mkdir -p $(LCOV_DIR)
	@echo "Capturing coverage data..."
	lcov --capture --directory $(OBJ_DIR) --output-file $(LCOV_INFO) --rc branch_coverage=1 --ignore-errors mismatch,gcov,gcov
	@echo "Filtering out system, Boost, and test files..."
# lcov --remove $(LCOV_INFO) "/usr/*" "*/test/*" "*/boost/*" --output-file $(LCOV_INFO) --rc branch_coverage=1 --ignore-errors unused
	lcov --remove $(LCOV_INFO) "/usr/*" "*/test/*" "*/boost/*" "$(SRC_DIR)/main.cpp" --output-file $(LCOV_INFO) --rc branch_coverage=1 --ignore-errors unused
	@echo "Generating HTML report..."
	genhtml $(LCOV_INFO) --output-directory $(HTML_OUTPUT) --rc branch_coverage=1
	@echo "HTML report generated at $(HTML_OUTPUT)/index.html"



# Clean coverage data
clean-gcov:
	find $(OBJ_DIR) -type f -name "*.gcda" -delete
	find $(OBJ_DIR) -type f -name "*.gcno" -delete
	rm -rf $(GCOV_DIR)

clean-lcov:
	rm -rf $(LCOV_DIR)

.PHONY: all clean test run gcov lcov
# Compiler and Flags
# Compiler and Flags
# # Output file
# TARGET = bin/MusicPlayer
# TEST_TARGET = bin/test_runner

# # Compiler and Flags
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -g -I/usr/local/include -I/usr/include \
#            -I/usr/local/include/SDL2 -Iinclude -fprofile-arcs -ftest-coverage
# LDFLAGS = -L/usr/local/lib -L/usr/lib -L/lib/x86_64-linux-gnu \
#           -lSDL2 -lSDL2_mixer -lSDL2_ttf -ltag -lz \
#           -lavcodec -lavformat -lavutil -lswscale \
#           -lftxui-component -lftxui-dom -lftxui-screen \
#           -pthread -ldl -lgtest -lgtest_main -lgmock -lgmock_main \
#           -fprofile-arcs -ftest-coverage

# # Directories
# SRC_DIR = src
# TEST_DIR = test
# OBJ_DIR = build/objects
# BIN_DIR = bin

# # Sources
# MAIN_SOURCE = $(SRC_DIR)/main.cpp
# SOURCES := $(filter-out $(MAIN_SOURCE), $(shell find $(SRC_DIR) -type f -name "*.cpp"))
# TEST_SOURCES := $(shell find $(TEST_DIR) -type f -name "*.cpp")

# # Objects
# OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/src/%.o, $(SOURCES))
# MAIN_OBJECT := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/src/%.o, $(MAIN_SOURCE))
# TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/test/%.o, $(TEST_SOURCES))

# # Executables
# EXEC = $(BIN_DIR)/MusicPlayer
# TEST_EXEC = $(BIN_DIR)/test_runner

# # Default target
# all: $(EXEC)

# # Build the main application
# $(EXEC): $(OBJECTS) $(MAIN_OBJECT)
# 	@mkdir -p $(BIN_DIR)
# 	$(CXX) $(OBJECTS) $(MAIN_OBJECT) -o $@ $(LDFLAGS)

# # Build the main.cpp file
# $(MAIN_OBJECT): $(MAIN_SOURCE)
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Build and run tests (UPDATED)
# test: $(TEST_EXEC)
# 	@echo "Cleaning previous coverage data..."
# 	find . -name '*.gcda' -exec rm -f {} +
# 	@echo "Running tests..."
# 	@./$(TEST_EXEC)

# # Build the test executable
# $(TEST_EXEC): $(OBJECTS) $(TEST_OBJECTS)
# 	@mkdir -p $(BIN_DIR)
# 	$(CXX) $(TEST_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)

# # Compile source files into object files
# $(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Compile test files into object files
# $(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean up build artifacts
# clean:
# 	rm -rf $(OBJ_DIR) $(BIN_DIR) coverage coverage-lcov

# # Generate coverage report (UPDATED)
# LCOV_DIR = coverage-lcov
# LCOV_INFO = $(LCOV_DIR)/coverage.info
# HTML_OUTPUT = $(LCOV_DIR)/html

# coverage: test
# 	@mkdir -p $(LCOV_DIR)
# 	@echo "Capturing coverage data..."
# 	lcov --capture --directory $(OBJ_DIR)/src --output-file $(LCOV_INFO) --rc branch_coverage=1 --ignore-errors mismatch
# 	@echo "Filtering out system, test, and external files..."
# 	lcov --remove $(LCOV_INFO) "/usr/*" "*/test/*" "*/external_libs/*" --output-file $(LCOV_INFO) --rc branch_coverage=1
# 	@echo "Generating HTML report..."
# 	genhtml $(LCOV_INFO) --output-directory $(HTML_OUTPUT) --rc branch_coverage=1
# 	@echo "Coverage report generated in $(HTML_OUTPUT)/index.html"

# # Reset coverage data
# reset:
# 	@echo "Resetting coverage data..."
# 	find . -name '*.gcda' -exec rm -f {} +
# 	find . -name '*.gcno' -exec rm -f {} +
# 	rm -rf coverage coverage-lcov

# .PHONY: all clean test coverage reset
