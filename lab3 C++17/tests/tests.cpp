// tests.cpp
//
// lab3 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include <gtest/gtest.h>
#include "../includes/marker_array.h"
#include <thread>
#include <vector>
#include <algorithm>

TEST(MarkerArrayTest, StartsWithZeros) {
    MarkerArray arr(5);
    auto snapshot = arr.copy();
    for (int value : snapshot) {
        EXPECT_EQ(value, 0);
    }
}

TEST(MarkerArrayTest, TryMarkSuccess) {
    MarkerArray arr(3);
    EXPECT_TRUE(arr.try_mark(1, 2));
    EXPECT_EQ(arr.get(1), 2);
}

TEST(MarkerArrayTest, TryMarkFailsWhenOccupied) {
    MarkerArray arr(3);
    arr.try_mark(1, 1);
    EXPECT_FALSE(arr.try_mark(1, 2));
    EXPECT_EQ(arr.get(1), 1);
}

TEST(MarkerArrayTest, ClearMarksOnlyOwn) {
    MarkerArray arr(5);
    arr.try_mark(0, 1);
    arr.try_mark(1, 2);
    arr.try_mark(2, 1);
    arr.try_mark(3, 2);
    
    arr.clear_marks(1);
    
    EXPECT_EQ(arr.get(0), 0);
    EXPECT_EQ(arr.get(1), 2);
    EXPECT_EQ(arr.get(2), 0);
    EXPECT_EQ(arr.get(3), 2);
}

TEST(MarkerArrayTest, CountMarkedBy) {
    MarkerArray arr(6);
    arr.try_mark(0, 3);
    arr.try_mark(1, 3);
    arr.try_mark(2, 2);
    arr.try_mark(4, 3);
    
    EXPECT_EQ(arr.count_marked_by(3), 3);
    EXPECT_EQ(arr.count_marked_by(2), 1);
    EXPECT_EQ(arr.count_marked_by(1), 0);
}

TEST(MarkerArrayTest, GetOutOfBoundsReturnsMinusOne) {
    MarkerArray arr(3);
    EXPECT_EQ(arr.get(100), -1);
}

TEST(MarkerArrayTest, TryMarkOutOfBoundsReturnsFalse) {
    MarkerArray arr(3);
    EXPECT_FALSE(arr.try_mark(100, 1));
}

TEST(MarkerArrayTest, ClearNonExistentIdSafe) {
    MarkerArray arr(4);
    arr.try_mark(0, 1);
    arr.try_mark(1, 1);
    arr.clear_marks(999);
    EXPECT_EQ(arr.get(0), 1);
    EXPECT_EQ(arr.get(1), 1);
}

TEST(MarkerArrayTest, DoubleClearSafe) {
    MarkerArray arr(3);
    arr.try_mark(0, 5);
    arr.clear_marks(5);
    arr.clear_marks(5);
    EXPECT_EQ(arr.get(0), 0);
}

TEST(MarkerArrayTest, CopyReturnsIndependentCopy) {
    MarkerArray arr(3);
    arr.try_mark(0, 1);
    arr.try_mark(2, 2);
    
    auto snapshot = arr.copy();
    snapshot[0] = 999;
    snapshot[1] = 888;
    
    EXPECT_EQ(arr.get(0), 1);
    EXPECT_EQ(arr.get(2), 2);
    EXPECT_EQ(snapshot[0], 999);
}

TEST(MarkerArrayTest, SizeReturnsCorrectValue) {
    MarkerArray arr(10);
    EXPECT_EQ(arr.size(), 10);
}

TEST(MarkerArrayStressTest, ConcurrentTryMarkThreadSafe) {
    MarkerArray arr(100);
    const int THREADS = 10;
    std::vector<std::thread> workers;
    
    for (int id = 1; id <= THREADS; ++id) {
        workers.emplace_back([&arr, id]() {
            for (int i = 0; i < 500; ++i) {
                std::size_t idx = static_cast<std::size_t>(i % arr.size());
                arr.try_mark(idx, id);
            }
        });
    }
    
    for (auto& worker : workers) {
        worker.join();
    }
    
    auto snapshot = arr.copy();
    for (int value : snapshot) {
        EXPECT_TRUE(value >= 0 && value <= THREADS);
    }
}

TEST(MarkerArrayStressTest, ConcurrentClearAndMark) {
    MarkerArray arr(50);
    std::vector<std::thread> workers;
    
    workers.emplace_back([&arr]() {
        for (int i = 0; i < 1000; ++i) {
            arr.try_mark(static_cast<std::size_t>(i % 50), 1);
        }
    });
    
    workers.emplace_back([&arr]() {
        for (int i = 0; i < 200; ++i) {
            arr.clear_marks(1);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });
    
    for (auto& worker : workers) {
        worker.join();
    }
    
    auto snapshot = arr.copy();
    EXPECT_EQ(snapshot.size(), 50);
    for (int value : snapshot) {
        EXPECT_TRUE(value == 0 || value == 1);
    }
}