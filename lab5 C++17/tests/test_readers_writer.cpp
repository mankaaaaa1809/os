// test_readers_writer.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "gtest/gtest.h"
#include "../includes/readers_writer.h"
#include <thread>
#include <vector>

TEST(ReadersWriterTest, MultipleReadersAllowed) {
    RecordLocks locks(1);
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&locks] {
            locks.lock_read(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            locks.unlock_read(0);
        });
    }
    
    for (auto& t : threads) t.join();
    SUCCEED();
}

TEST(ReadersWriterTest, WriterExclusive) {
    RecordLocks locks(1);
    bool writer_done = false;
    
    std::thread writer([&locks, &writer_done] {
        locks.lock_write(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        writer_done = true;
        locks.unlock_write(0);
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_FALSE(writer_done);
    
    writer.join();
    EXPECT_TRUE(writer_done);
}