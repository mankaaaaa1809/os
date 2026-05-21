// file_handler.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "file_handler.h"
#include <fstream>
#include <iostream>

FileHandler::FileHandler(const std::string& filename) : _filename(filename) {}

void FileHandler::CreateFileWithData(const std::vector<Employee>& data) {
    std::ofstream ofs(_filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) return;

    for (const auto& e : data) {
        ofs.write(reinterpret_cast<const char*>(&e.num), sizeof(int));
        unsigned int name_len = static_cast<unsigned int>(e.name.size());
        ofs.write(reinterpret_cast<const char*>(&name_len), sizeof(unsigned int));
        if (name_len > 0) ofs.write(e.name.c_str(), name_len);
        ofs.write(reinterpret_cast<const char*>(&e.hours), sizeof(double));
    }
    ofs.close();
}

void FileHandler::ReadAllAndPrint() {
    std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
    if (!ifs.is_open()) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    std::cout << "\n--- File Contents ---" << std::endl;
    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        std::string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        Employee e(num, name, hours);
        e.Print();
    }
    std::cout << "---------------------\n" << std::endl;
    ifs.close();
}

bool FileHandler::GetEmployee(int id, Employee& outEmp) {
    std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
    if (!ifs.is_open()) return false;

    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        std::string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        if (num == id) {
            outEmp = Employee(num, name, hours);
            ifs.close();
            return true;
        }
    }
    ifs.close();
    return false;
}

bool FileHandler::UpdateEmployee(const Employee& emp) {
    std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
    if (!ifs.is_open()) return false;

    std::vector<Employee> all;
    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        std::string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        all.push_back(Employee(num, name, hours));
    }
    ifs.close();

    bool found = false;
    for (auto& e : all) {
        if (e.num == emp.num) {
            e = emp;
            found = true;
            break;
        }
    }
    if (!found) return false;

    CreateFileWithData(all);
    return true;
}

void FileHandler::LoadFromFile(std::vector<Employee>& employees) {
    std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
    if (!ifs.is_open()) return;

    employees.clear();
    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        std::string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        employees.push_back(Employee(num, name, hours));
    }
    ifs.close();
}

void FileHandler::SaveToFile(const std::vector<Employee>& employees) {
    CreateFileWithData(employees);
}