// file_handler.h
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "employee.h"
#include <string>
#include <vector>

class FileHandler {
public:
    explicit FileHandler(const std::string& filename);
    void CreateFileWithData(const std::vector<Employee>& data);
    void ReadAllAndPrint();
    bool GetEmployee(int id, Employee& outEmp);
    bool UpdateEmployee(const Employee& emp);
    void LoadFromFile(std::vector<Employee>& employees);
    void SaveToFile(const std::vector<Employee>& employees);

private:
    std::string _filename;
};

#endif