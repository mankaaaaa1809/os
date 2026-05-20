## Laboratory work 1

## Description
This console-based Windows application demonstrates inter-process communication via file mapping. The "Main" program spawns two utilities: "Creator", which generates a binary file from user input, and "Reporter", which reads that binary file to produce a formatted text report with calculated wages. No direct process linking is used; data exchange is handled exclusively through the file system.

## Installation
1. Clone the repository.
2. Open the solution in Visual Studio.
3. Ensure all source files (Main.cpp, Creator.cpp, Reporter.cpp, employee.cpp) are included.
4. Build the solution.

## Usage
Run "Main.exe". Enter the binary file name, number of employees, report file name, and hourly rate. The program automatically launches "Creator.exe" and "Reporter.exe", waits for their completion, and displays the results on the console.

## Links and Additional Notes
This lab focuses on WinAPI process management using CreateProcess and WaitForSingleObject. No external libraries are required.
