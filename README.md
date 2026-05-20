## "Windows Thread Synchronization Lab"

## Description
This console application demonstrates multithreading in Windows. It creates an integer array, then launches two threads: one to find the minimum and maximum values, and another to calculate the average. After both threads complete, the main thread replaces all occurrences of the min and max elements with the average value and outputs the modified array. The implementation uses WinAPI threads, mutexes, and console synchronization.

## Installation
1. Ensure you have a C++17 compatible compiler (e.g., g++ 15.2).
2. Clone the repository.
3. Navigate to the project root folder.

## Usage
```bash
make all
make run
```

## Links and additional notes
This lab focuses on thread creation, synchronization, and shared resource protection using mutexes. No external libraries are required.
