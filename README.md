## Overview
This laboratory work implements a multithreaded console application where multiple marker threads concurrently attempt to write their IDs into shared integer array cells. The main thread coordinates marker lifecycle: starting all markers simultaneously, waiting until all become blocked, terminating a selected marker, and resuming the remaining ones. The implementation requires careful thread synchronization using mutexes and condition variables, handling deadlock situations, and ensuring memory safety during concurrent access.

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
