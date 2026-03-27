#include <iostream>
#include <fstream>
#include <iomanip>
#include "employee.h"

using namespace std;

int main(int argc, char* args[]) {
    if (argc != 3) {
        cerr << "Incorrect number of arguments\n";
        return 1;
    }

    const char* binaryFilename = args[0];
    const char* reportFilename = args[1];
    double payRate = atof(args[2]);

    ifstream fin(binaryFilename, ios::binary);
    ofstream fout(reportFilename);

    if (!fin || !fout) {
        cerr << "Couldn't open file\n";
        return 1;
    }

    fout << "Report " << binaryFilename << "\n";
    fout << "ID\tName\tHours\tSalary\n";

    employee e;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(employee))) {
        double wage = countWage(e.hours, payRate);
        fout << e.num << "\t" << e.name << "\t" << e.hours << "\t" << wage << "\n";
    }

    fin.close();
    fout.close();

    return 0;
}