// test_queue.cpp
//
// lab4 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include <gtest/gtest.h>
#include "file_queue.h"
#include <string>

TEST(FileQueueTest, PushPopBasic) {
    FileQueue q("test_basic.bin", 3);
    q.push("hello");
    q.push("world");
    EXPECT_EQ(q.pop(), "hello");
    EXPECT_EQ(q.pop(), "world");
}

TEST(FileQueueTest, FIFOOrder) {
    FileQueue q("test_fifo.bin", 5);
    q.push("first");
    q.push("second");
    q.push("third");
    EXPECT_EQ(q.pop(), "first");
    EXPECT_EQ(q.pop(), "second");
    EXPECT_EQ(q.pop(), "third");
}

TEST(FileQueueTest, MaxLengthMessage) {
    FileQueue q("test_maxlen.bin", 2);
    std::string longMsg(20, 'a');
    q.push(longMsg);
    EXPECT_EQ(q.pop(), longMsg);
}

TEST(FileQueueTest, TooLongMessageThrows) {
    FileQueue q("test_toolong.bin", 2);
    std::string tooLong(21, 'b');
    EXPECT_THROW(q.push(tooLong), std::runtime_error);
}

TEST(FileQueueTest, EmptyMessageThrows) {
    FileQueue q("test_empty_msg.bin", 2);
    EXPECT_THROW(q.push(""), std::runtime_error);
}

TEST(FileQueueTest, WrapAround) {
    FileQueue q("test_wrap.bin", 2);
    q.push("one");
    q.push("two");
    EXPECT_EQ(q.pop(), "one");
    q.push("three");
    EXPECT_EQ(q.pop(), "two");
    EXPECT_EQ(q.pop(), "three");
}