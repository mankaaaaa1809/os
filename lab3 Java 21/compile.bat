@echo off
set SRC_DIR=src
set BIN_DIR=bin

if not exist %BIN_DIR% mkdir %BIN_DIR%

echo Compiling...
javac -d %BIN_DIR% %SRC_DIR%/*.java

if errorlevel 1 (
    echo Compilation failed
    pause
    exit /b 1
)

echo Compilation successful
pause