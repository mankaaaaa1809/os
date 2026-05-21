CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iincludes
LDFLAGS = -static

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SERVER_SOURCES = $(SRC_DIR)/server.cpp \
                 $(SRC_DIR)/employee.cpp \
                 $(SRC_DIR)/file_handler.cpp \
                 $(SRC_DIR)/request.cpp \
                 $(SRC_DIR)/readers_writer.cpp \
                 $(SRC_DIR)/pipe_name.cpp

CLIENT_SOURCES = $(SRC_DIR)/client.cpp \
                 $(SRC_DIR)/employee.cpp \
                 $(SRC_DIR)/request.cpp \
                 $(SRC_DIR)/pipe_name.cpp

SERVER_OBJECTS = $(SERVER_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CLIENT_OBJECTS = $(CLIENT_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(BIN_DIR)/server.exe $(BIN_DIR)/client.exe

$(OBJ_DIR):
	if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/server.exe: $(SERVER_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/client.exe: $(CLIENT_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN_DIR)/server.exe $(BIN_DIR)/client.exe
	$(BIN_DIR)/server.exe

run-client: $(BIN_DIR)/client.exe
	$(BIN_DIR)/client.exe

clean:
	if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	if exist "$(BIN_DIR)\\server.exe" del /q "$(BIN_DIR)\\server.exe"
	if exist "$(BIN_DIR)\\client.exe" del /q "$(BIN_DIR)\\client.exe"

rebuild: clean all

.PHONY: all run run-client clean rebuild