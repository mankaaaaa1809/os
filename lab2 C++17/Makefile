CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iincludes
LDFLAGS = -lpthread

SRC_DIR = src
INC_DIR = includes
BIN_DIR = bin

TARGET = $(BIN_DIR)/main.exe

SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/data_input.cpp \
          $(SRC_DIR)/range_finder.cpp \
          $(SRC_DIR)/average_calculator.cpp

OBJECTS = $(BIN_DIR)/main.o \
          $(BIN_DIR)/data_input.o \
          $(BIN_DIR)/range_finder.o \
          $(BIN_DIR)/average_calculator.o

HEADERS = $(INC_DIR)/data_input.h \
          $(INC_DIR)/range_finder.h \
          $(INC_DIR)/average_calculator.h

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

$(BIN_DIR)/data_input.o: $(SRC_DIR)/data_input.cpp $(INC_DIR)/data_input.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/range_finder.o: $(SRC_DIR)/range_finder.cpp $(INC_DIR)/range_finder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/average_calculator.o: $(SRC_DIR)/average_calculator.cpp $(INC_DIR)/average_calculator.h
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
	@if not exist "$(SRC_DIR)/data_input.cpp" (echo Error: data_input.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/range_finder.cpp" (echo Error: range_finder.cpp not found! && exit 1)
	@if not exist "$(SRC_DIR)/average_calculator.cpp" (echo Error: average_calculator.cpp not found! && exit 1)
	@if not exist "$(INC_DIR)/data_input.h" (echo Error: data_input.h not found! && exit 1)
	@if not exist "$(INC_DIR)/range_finder.h" (echo Error: range_finder.h not found! && exit 1)
	@if not exist "$(INC_DIR)/average_calculator.h" (echo Error: average_calculator.h not found! && exit 1)
	@echo "All files present!"

.PHONY: all clean clean-all run run-args rebuild check