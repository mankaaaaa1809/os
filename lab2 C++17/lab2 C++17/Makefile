CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

TARGET = $(BIN_DIR)/main.exe

SOURCES = $(SRC_DIR)/mainProgram.cpp \
          $(SRC_DIR)/dataInput.cpp \
          $(SRC_DIR)/rangeFinder.cpp \
          $(SRC_DIR)/averageCalculator.cpp

OBJECTS = $(BIN_DIR)/main.o \
          $(BIN_DIR)/dataInput.o \
          $(BIN_DIR)/rangeFinder.o \
          $(BIN_DIR)/averageCalculator.o

HEADERS = $(INC_DIR)/dataInput.h \
          $(INC_DIR)/rangeFinder.h \
          $(INC_DIR)/averageCalculator.h

all: $(BIN_DIR) $(TARGET)
	@echo "Build complete! Executable in $(BIN_DIR)/"
	@echo "Run with: make run"

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Created: $@"

$(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/dataInput.o: $(SRC_DIR)/dataInput.cpp $(INC_DIR)/dataInput.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/rangeFinder.o: $(SRC_DIR)/rangeFinder.cpp $(INC_DIR)/rangeFinder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/averageCalculator.o: $(SRC_DIR)/averageCalculator.cpp $(INC_DIR)/averageCalculator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	-del /f $(BIN_DIR)\*.o 2>nul || rm -f $(BIN_DIR)/*.o
	-del /f $(TARGET) 2>nul || rm -f $(TARGET)
	@echo "Clean complete"

clean-all: clean
	@echo "Removing bin directory..."
	-del /f $(BIN_DIR)\* 2>nul || rm -f $(BIN_DIR)/*
	@echo "All cleaned"

run: $(TARGET)
	@echo "Running main.exe from $(BIN_DIR)/..."
	cd $(BIN_DIR) && .\main.exe

run-args: $(TARGET)
	cd $(BIN_DIR) && .\main.exe $(ARGS)

rebuild: clean all

check:
	@echo "Checking project structure..."
	@if not exist "$(SRC_DIR)" (echo Error: $(SRC_DIR) not found! && exit 1)
	@if not exist "$(INC_DIR)" (echo Error: $(INC_DIR) not found! && exit 1)
	@if not exist "$(SRC_DIR)/main.cpp" (echo Error: main.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/dataInput.cpp" (echo Error: dataInput.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/rangeFinder.cpp" (echo Error: rangeFinder.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/averageCalculator.cpp" (echo Error: averageCalculator.cpp not found! && exit 1)
	@if not exist "$(INC_DIR)/dataInput.h" (echo Error: dataInput.h not found! && exit 1)
	@if not exist "$(INC_DIR)/rangeFinder.h" (echo Error: rangeFinder.h not found! && exit 1)
	@if not exist "$(INC_DIR)/averageCalculator.h" (echo Error: averageCalculator.h not found! && exit 1)
	@echo "All files present!"

.PHONY: all clean clean-all run run-args rebuild check