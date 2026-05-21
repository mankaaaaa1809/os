## Laboratory work 4

## Description
A C++17 implementation of interprocess communication using a single binary file as a ring buffer. Multiple sender processes transmit messages (max 20 chars) to one receiver process. Synchronization uses WinAPI named mutex, semaphores, and event objects. The receiver waits for all senders to become ready and supports graceful shutdown.

## Installation
Create directories: `mkdir obj bin`. Run `make` to build `receiver.exe` and `sender.exe`. Requires MinGW or MSVC with C++17 support.

## Usage
``` cmd
make all
cd bin
receiver.exe
```

## Links and Additional Notes
Windows only. Queue blocks when empty (receiver) or full (sender). Tested with MinGW GCC.
