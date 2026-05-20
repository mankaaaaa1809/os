## Laboratory work 3

## Description
A console application demonstrating thread synchronization using mutexes and condition variables. Multiple marker threads concurrently write their IDs into a shared integer array. The main thread coordinates marker lifecycle: starts all markers simultaneously, waits until all become blocked, terminates a selected marker, clears its marks, and resumes remaining markers. Implements deadlock-free coordination with atomic flags and per-thread condition variables.

## Installation
Clone repository. For C++: ensure GCC with C++17 or Visual Studio 2022. Run `make all`. For Java: ensure JDK 21+, download JUnit Platform Console Standalone 1.10.0 to `lib/` folder, run `compile.bat`.

## Usage
C++: `make run` or `./bin/lab3.exe`. Java: `run.bat`. Tests: C++ `make test`, Java `test_console.bat`.

## Links and Additional Notes
Requires C++17 threads and Java Concurrency API. No external dependencies except JUnit for Java tests. Developed for educational purposes on thread synchronization patterns.

## Java and C++ versions comparison

| Characteristic | C++17 Implementation | Java 21+ Implementation |
|---|---|---|
| Total lines of code (excluding tests) | ~350 lines (4 files: 2 headers, 2 sources) | ~320 lines (4 files, no header separation) |
| Cyclomatic complexity (marker_worker) | 12 (nested loops, 4 condition branches, 3 state flags) | 12 (identical control flow with try-catch blocks) |
| Cyclomatic complexity (wait_all_blocked) | 6 (loop with predicate, atomic checks) | 6 (same logic with Lock/Condition) |
| Total project cyclomatic complexity | 38 | 35 |
| Memory management approach | RAII with shared_ptr, automatic lock_guard | Garbage collection, try-finally for locks |
| Thread synchronization primitives | std::mutex, std::condition_variable, std::atomic | ReentrantLock, Condition, AtomicBoolean |
| Exception safety | Strong guarantee via RAII, join in destructor | Basic guarantee, requires explicit finally blocks |
| Code coverage requirement | 90% line, 100% branch for synchronization paths | 90% line, 100% branch for Condition predicates |
| Test framework | Google Test (12 unit tests + stress tests) | JUnit 5 (12 unit tests + stress tests) |
| Build & run complexity | Single Makefile, no external dependencies for main program | Requires JUnit JAR download (10 MB) for tests, classpath configuration |

## Conclusion

The C++ implementation is better suited for real-time or performance-critical environments where predictable latency matters. The Java implementation offers easier portability across platforms and simpler debugging of concurrency issues through JVM tooling. Both satisfy all laboratory requirements and demonstrate correct multithreaded synchronization patterns.
