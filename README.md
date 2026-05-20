## Laboratory work 1

## Description
This console-based Windows application demonstrates inter-process communication via file mapping. The "Main" program spawns two utilities: "Creator", which generates a binary file from user input, and "Reporter", which reads that binary file to produce a formatted text report with calculated wages. No direct process linking is used; data exchange is handled exclusively through the file system.

## Installation
1. Ensure you have a C++11 compatible compiler
2. Clone the repository.
3. Navigate to the project root folder.

## Usage
```bash
make all
make run
```

## Links and Additional Notes
This lab focuses on WinAPI process management using CreateProcess and WaitForSingleObject. No external libraries are required.
