// test_employee.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "gtest/gtest.h"
#include "../includes/employee.h"

TEST(EmployeeTest, ConstructorWithParams) {
    Employee e(1, "John", 40.5);
    EXPECT_EQ(e.num, 1);
    EXPECT_EQ(e.name, "John");
    EXPECT_DOUBLE_EQ(e.hours, 40.5);
}

TEST(EmployeeTest, DefaultConstructor) {
    Employee e;
    EXPECT_EQ(e.num, 0);
    EXPECT_EQ(e.name, "");
    EXPECT_DOUBLE_EQ(e.hours, 0.0);
}