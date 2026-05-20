@echo off
set BIN_DIR=bin

if not exist %BIN_DIR% (
    echo Please run compile.bat first
    pause
    exit /b 1
)

echo Running...
java -cp %BIN_DIR% src.Main

pause