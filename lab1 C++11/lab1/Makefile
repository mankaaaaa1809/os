CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
DOCS_DIR = docs

TARGETS = $(BIN_DIR)/creator.exe $(BIN_DIR)/reporter.exe $(BIN_DIR)/main.exe

CREATOR_SRC = $(SRC_DIR)/creator.cpp
REPORTER_SRC = $(SRC_DIR)/reporter.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp
COMMON_SRC = $(SRC_DIR)/employee.cpp

CREATOR_OBJ = $(BIN_DIR)/creator.o
REPORTER_OBJ = $(BIN_DIR)/reporter.o
MAIN_OBJ = $(BIN_DIR)/main.o
COMMON_OBJ = $(BIN_DIR)/employee.o

HEADERS = $(INC_DIR)/employee.h

all: $(BIN_DIR) $(DOCS_DIR) $(TARGETS)
	@echo "Build complete! Executables in $(BIN_DIR)/"
	@echo "Data files will be created in $(DOCS_DIR)/"
	@echo "Run with: make run"

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(DOCS_DIR):
	mkdir -p $(DOCS_DIR)

$(BIN_DIR)/creator.exe: $(CREATOR_OBJ) $(COMMON_OBJ)
	$(CXX) $(CREATOR_OBJ) $(COMMON_OBJ) -o $@ $(LDFLAGS)
	@echo "Created: $@"

$(BIN_DIR)/reporter.exe: $(REPORTER_OBJ) $(COMMON_OBJ)
	$(CXX) $(REPORTER_OBJ) $(COMMON_OBJ) -o $@ $(LDFLAGS)
	@echo "Created: $@"

$(BIN_DIR)/main.exe: $(MAIN_OBJ) $(COMMON_OBJ)
	$(CXX) $(MAIN_OBJ) $(COMMON_OBJ) -o $@ $(LDFLAGS)
	@echo "Created: $@"

$(CREATOR_OBJ): $(CREATOR_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(REPORTER_OBJ): $(REPORTER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(COMMON_OBJ): $(COMMON_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	-del /f *.o 2>nul || rm -f *.o
	-del /f $(BIN_DIR)\*.exe 2>nul || rm -f $(BIN_DIR)/*.exe
	@echo "Clean complete"

clean-all: clean
	@echo "Cleaning docs..."
	-del /f $(DOCS_DIR)\* 2>nul || rm -f $(DOCS_DIR)/*
	@echo "All cleaned"

run: $(BIN_DIR)/main.exe $(DOCS_DIR)
	@echo "Running main.exe from $(BIN_DIR)/..."
	cd $(BIN_DIR) && .\main.exe

run-args: $(BIN_DIR)/main.exe $(DOCS_DIR)
	cd $(BIN_DIR) && .\main.exe $(ARGS)

rebuild: clean all

check:
	@echo "Checking project structure..."
	@if not exist "$(SRC_DIR)" (echo Error: $(SRC_DIR) not found! && exit 1)
	@if not exist "$(INC_DIR)" (echo Error: $(INC_DIR) not found! && exit 1)
	@if not exist "$(SRC_DIR)/main.cpp" (echo Error: main.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/creator.cpp" (echo Error: creator.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/reporter.cpp" (echo Error: reporter.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/employee.cpp" (echo Error: employee.cpp not found! && exit 1)
	@if not exist "$(INC_DIR)/employee.h" (echo Error: employee.h not found! && exit 1)
	@echo "All files present!"

.PHONY: all clean clean-all run run-args rebuild check