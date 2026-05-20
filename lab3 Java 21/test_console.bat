@echo off
set BIN_DIR=bin
set LIB_DIR=lib

if not exist %BIN_DIR% mkdir %BIN_DIR%

echo Compiling...
javac -d %BIN_DIR% -cp "%LIB_DIR%\junit-platform-console-standalone-1.10.0.jar" src/*.java tests/*.java

if errorlevel 1 (
    echo Compilation failed
    pause
    exit /b 1
)

echo Running tests...
java -jar %LIB_DIR%\junit-platform-console-standalone-1.10.0.jar --class-path %BIN_DIR% --scan-class-path

pause