## Laboratory work 5

## Description
A C++17 implementation of a multithreaded server and multiple clients communicating via Windows named pipes. The server manages a binary file with employee records (ID, name, hours) and implements readers-writer locks per record. Multiple readers can access the same record simultaneously, while writers have exclusive access. The server spawns client processes, handles parallel requests, and displays the final file state after all clients finish.

## Installation
Clone the repository. Ensure MinGW with g++ and make are installed. Place all .h files in includes/ and all .cpp files in src/. Run `make all` to build server.exe and client.exe.

## Usage
``` cmd
make all
make run
```

## Links and Additional Notes
Requires Windows OS and C++17. Uses WinAPI for pipes and threads.
