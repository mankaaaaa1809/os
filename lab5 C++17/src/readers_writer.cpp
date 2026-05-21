// readers_writer.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "readers_writer.h"

ReadersWriterLock::ReadersWriterLock() : read_count(0) {}

void ReadersWriterLock::lock_read() {
    std::lock_guard<std::mutex> lk(read_count_mtx);
    read_count++;
    if (read_count == 1) {
        resource_mtx.lock();
    }
}

void ReadersWriterLock::unlock_read() {
    std::lock_guard<std::mutex> lk(read_count_mtx);
    read_count--;
    if (read_count == 0) {
        resource_mtx.unlock();
    }
}

void ReadersWriterLock::lock_write() {
    resource_mtx.lock();
}

void ReadersWriterLock::unlock_write() {
    resource_mtx.unlock();
}

RecordLocks::RecordLocks(int size) {
    for (int i = 0; i < size; ++i) {
        locks.push_back(std::make_unique<ReadersWriterLock>());
    }
}

void RecordLocks::lock_read(int index) {
    if (index >= 0 && index < static_cast<int>(locks.size())) {
        locks[index]->lock_read();
    }
}

void RecordLocks::unlock_read(int index) {
    if (index >= 0 && index < static_cast<int>(locks.size())) {
        locks[index]->unlock_read();
    }
}

void RecordLocks::lock_write(int index) {
    if (index >= 0 && index < static_cast<int>(locks.size())) {
        locks[index]->lock_write();
    }
}

void RecordLocks::unlock_write(int index) {
    if (index >= 0 && index < static_cast<int>(locks.size())) {
        locks[index]->unlock_write();
    }
}