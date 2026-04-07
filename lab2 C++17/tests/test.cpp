#include <gtest/gtest.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "../lab2 C++17/include/dataInput.h"
#include "../lab2 C++17/include/rangeFinder.h"
#include "../lab2 C++17/include/averageCalculator.h"

class ArrayProcessorTest : public ::testing::Test {
protected:
    void TearDown() override {
    }
};

TEST_F(ArrayProcessorTest, HandlesPositiveNumbersCorrectly) {
    std::string inputData = "5\n10 20 30 40 50\n";
    std::stringstream ssInput(inputData);
    auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

    auto numbers = readConsoleData();
    std::cin.rdbuf(oldCin);

    HANDLE hRange = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

    WaitForSingleObject(hRange, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hRange);
    CloseHandle(hAverage);

    EXPECT_EQ(globalRangeResult.smallest, 10);
    EXPECT_EQ(globalRangeResult.largest, 50);
    EXPECT_DOUBLE_EQ(globalAverageResult.value, 30.0);

    for (auto& val : numbers) {
        if (val == globalRangeResult.smallest || val == globalRangeResult.largest)
            val = static_cast<long long>(globalAverageResult.value);
    }

    std::stringstream ssOutput;
    auto oldCout = std::cout.rdbuf(ssOutput.rdbuf());
    showArray(numbers);
    std::cout.rdbuf(oldCout);

    std::string result = ssOutput.str();
    EXPECT_NE(result.find("30"), std::string::npos);
}

TEST_F(ArrayProcessorTest, HandlesNegativeNumbersCorrectly) {
    std::string inputData = "6\n-15 -30 -45 -60 -75 -90\n";
    std::stringstream ssInput(inputData);
    auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

    auto numbers = readConsoleData();

    std::cin.rdbuf(oldCin);

    HANDLE hRange = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

    WaitForSingleObject(hRange, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hRange);
    CloseHandle(hAverage);

    EXPECT_EQ(globalRangeResult.smallest, -90);
    EXPECT_EQ(globalRangeResult.largest, -15);
    EXPECT_DOUBLE_EQ(globalAverageResult.value, -52.5);
}

TEST_F(ArrayProcessorTest, HandlesSingleElementArray) {
    std::string inputData = "1\n42\n";
    std::stringstream ssInput(inputData);
    auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

    auto numbers = readConsoleData();

    std::cin.rdbuf(oldCin);

    HANDLE hRange = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

    WaitForSingleObject(hRange, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hRange);
    CloseHandle(hAverage);

    EXPECT_EQ(globalRangeResult.smallest, 42);
    EXPECT_EQ(globalRangeResult.largest, 42);
    EXPECT_DOUBLE_EQ(globalAverageResult.value, 42.0);
}

TEST_F(ArrayProcessorTest, HandlesMixedPositiveAndNegativeNumbers) {
    std::string inputData = "7\n-100 50 -25 75 -50 25 0\n";
    std::stringstream ssInput(inputData);
    auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

    auto numbers = readConsoleData();

    std::cin.rdbuf(oldCin);

    HANDLE hRange = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

    WaitForSingleObject(hRange, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hRange);
    CloseHandle(hAverage);

    EXPECT_EQ(globalRangeResult.smallest, -100);
    EXPECT_EQ(globalRangeResult.largest, 75);

    double expectedAverage = (-100.0 + 50.0 - 25.0 + 75.0 - 50.0 + 25.0 + 0.0) / 7.0;
    EXPECT_DOUBLE_EQ(globalAverageResult.value, expectedAverage);
}

TEST_F(ArrayProcessorTest, HandlesArrayWithDuplicateMinMaxValues) {
    std::string inputData = "8\n5 5 10 15 15 20 20 25\n";
    std::stringstream ssInput(inputData);
    auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

    auto numbers = readConsoleData();

    std::cin.rdbuf(oldCin);

    HANDLE hRange = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

    WaitForSingleObject(hRange, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hRange);
    CloseHandle(hAverage);

    EXPECT_EQ(globalRangeResult.smallest, 5);
    EXPECT_EQ(globalRangeResult.largest, 25);

    double expectedAverage = (5.0 + 5.0 + 10.0 + 15.0 + 15.0 + 20.0 + 20.0 + 25.0) / 8.0;
    EXPECT_DOUBLE_EQ(globalAverageResult.value, expectedAverage);
}

TEST_F(ArrayProcessorTest, HandlesReplacementOfMinAndMax) {
    std::string inputData = "5\n10 20 30 40 50\n";
    std::stringstream ssInput(inputData);
    auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

    auto numbers = readConsoleData();

    std::cin.rdbuf(oldCin);

    HANDLE hRange = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
    HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

    WaitForSingleObject(hRange, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hRange);
    CloseHandle(hAverage);

    for (auto& val : numbers) {
        if (val == globalRangeResult.smallest || val == globalRangeResult.largest) {
            val = static_cast<long long>(globalAverageResult.value);
        }
    }

    EXPECT_EQ(numbers[0], 30);
    EXPECT_EQ(numbers[1], 20);
    EXPECT_EQ(numbers[2], 30);
    EXPECT_EQ(numbers[3], 40);
    EXPECT_EQ(numbers[4], 30);

    TEST_F(ArrayProcessorTest, ThrowsExceptionForEmptyArray) {
        std::string inputData = "0\n";
        std::stringstream ssInput(inputData);
        auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

        EXPECT_THROW({
            auto numbers = readConsoleData();
            }, std::invalid_argument);

        std::cin.rdbuf(oldCin);
    }

    TEST_F(ArrayProcessorTest, ThrowsExceptionForInvalidInput) {
        std::string inputData = "5\nabc def ghi jkl mno\n";
        std::stringstream ssInput(inputData);
        auto oldCin = std::cin.rdbuf(ssInput.rdbuf());

        EXPECT_THROW({
            auto numbers = readConsoleData();
            }, std::invalid_argument);

        std::cin.rdbuf(oldCin);
    }

    int main(int argc, char** argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }