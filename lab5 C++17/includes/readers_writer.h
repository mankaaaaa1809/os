// readers_writer.h
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#ifndef READERS_WRITER_H
#define READERS_WRITER_H

#include <mutex>
#include <vector>
#include <memory>

struct ReadersWriterLock {
    std::mutex read_count_mtx;
    std::mutex resource_mtx;
    int read_count;

    ReadersWriterLock();
    ~ReadersWriterLock() = default;
    ReadersWriterLock(const ReadersWriterLock&) = delete;
    ReadersWriterLock& operator=(const ReadersWriterLock&) = delete;
    ReadersWriterLock(ReadersWriterLock&&) = delete;
    ReadersWriterLock& operator=(ReadersWriterLock&&) = delete;

    void lock_read();
    void unlock_read();
    void lock_write();
    void unlock_write();
};

class RecordLocks {
public:
    explicit RecordLocks(int size);
    void lock_read(int index);
    void unlock_read(int index);
    void lock_write(int index);
    void unlock_write(int index);

private:
    std::vector<std::unique_ptr<ReadersWriterLock>> locks;
};

#endif