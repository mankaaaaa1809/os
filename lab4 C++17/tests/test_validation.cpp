// test_validation.cpp
//
// lab4 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include <gtest/gtest.h>
#include <regex>
#include <string>

static bool is_positive_integer(const std::string& s) {
    static std::regex reg("^[1-9][0-9]*$");
    return std::regex_match(s, reg);
}

TEST(ValidationTest, ValidPositive) {
    EXPECT_TRUE(is_positive_integer("1"));
    EXPECT_TRUE(is_positive_integer("10"));
    EXPECT_TRUE(is_positive_integer("999"));
}

TEST(ValidationTest, InvalidZero) {
    EXPECT_FALSE(is_positive_integer("0"));
}

TEST(ValidationTest, InvalidNegative) {
    EXPECT_FALSE(is_positive_integer("-5"));
}

TEST(ValidationTest, InvalidLetters) {
    EXPECT_FALSE(is_positive_integer("abc"));
    EXPECT_FALSE(is_positive_integer("12a"));
}

TEST(ValidationTest, InvalidEmpty) {
    EXPECT_FALSE(is_positive_integer(""));
}

TEST(ValidationTest, InvalidFloat) {
    EXPECT_FALSE(is_positive_integer("1.5"));
}