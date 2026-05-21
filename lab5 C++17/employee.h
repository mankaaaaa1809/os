// employee.h
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

struct Employee {
    int num;
    std::string name;
    double hours;

    Employee();
    Employee(int n, const std::string& name_str, double h);
    void Print() const;
};

#endif