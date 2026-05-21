// test_file_handler.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "gtest/gtest.h"
#include "../includes/file_handler.h"
#include <cstdio>

class FileHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        filename = "test_employees.bin";
    }
    void TearDown() override {
        std::remove(filename.c_str());
    }
    std::string filename;
};

TEST_F(FileHandlerTest, CreateAndReadBack) {
    FileHandler fh(filename);
    std::vector<Employee> data = {Employee(1, "Alice", 50.0)};
    fh.CreateFileWithData(data);
    
    Employee result;
    ASSERT_TRUE(fh.GetEmployee(1, result));
    EXPECT_EQ(result.name, "Alice");
}