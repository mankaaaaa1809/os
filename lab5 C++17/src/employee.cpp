// employee.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "employee.h"
#include <iostream>

Employee::Employee() : num(0), name(""), hours(0.0) {}

Employee::Employee(int n, const std::string& name_str, double h)
    : num(n), name(name_str), hours(h) {}

void Employee::Print() const {
    std::cout << "ID: " << num << "\tName: " << name << "\tHours: " << hours << std::endl;
}