#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstdio>
#include "employee.h"

int main() {
	std::string docsPath = "../docs/";
	std::string binaryFile, fullBinaryFilename, reportFile, fullReportFilename;
	int count;
	double payRate;

	std::cout << "Enter a binary file name and number of employees: ";
	std::cin >> binaryFile >> count;

	fullBinaryFilename = docsPath + binaryFile;
	char creatorCmd[256];
	sprintf(creatorCmd, "%s %i", fullBinaryFilename.c_str(), count);

	STARTUPINFOA siCreator;
	ZeroMemory(&siCreator, sizeof(siCreator));
	siCreator.cb = sizeof(siCreator);
	PROCESS_INFORMATION piCreator;
	ZeroMemory(&piCreator, sizeof(piCreator));

	if (!CreateProcessA("../bin/creator.exe", creatorCmd, NULL, NULL, FALSE, 0, NULL, NULL, &siCreator, &piCreator)) {
		std::cerr << "Creator launching error\n";
		return 1;
	}
	WaitForSingleObject(piCreator.hProcess, INFINITE);
	CloseHandle(piCreator.hProcess);
	CloseHandle(piCreator.hThread);

	std::ifstream finBinary(fullBinaryFilename, std::ios::binary);
	if (!finBinary) {
		std::cerr << "Couldn't open " + fullBinaryFilename + '\n';
		return 1;
	}
	employee e;
	std::cout << binaryFile << '\n';
	while (finBinary.read(reinterpret_cast<char*>(&e), sizeof(e))) {
		std::cout << e.num << ": " << e.name << ", " << e.hours << '\n';
	}
	finBinary.close();

	std::cout << "Enter report filename and a pay rate: ";
	std::cin >> reportFile >> payRate;

	fullReportFilename = docsPath + reportFile;
	char reporterCmd[256];
	sprintf(reporterCmd, "%s %s %f", fullBinaryFilename.c_str(), fullReportFilename.c_str(), payRate);

	STARTUPINFOA siReporter;
	ZeroMemory(&siReporter, sizeof(siReporter));
	siReporter.cb = sizeof(siReporter);
	PROCESS_INFORMATION piReporter;
	ZeroMemory(&piReporter, sizeof(piReporter));

	if (!CreateProcessA("../bin/reporter.exe", reporterCmd, NULL, NULL, FALSE, 0, NULL, NULL, &siReporter, &piReporter)) {
		std::cerr << "Reporter launching error\n";
		return 1;
	}
	WaitForSingleObject(piReporter.hProcess, INFINITE);
	CloseHandle(piReporter.hProcess);
	CloseHandle(piReporter.hThread);

	std::ifstream finReport(fullReportFilename);
	if (!finReport) {
		std::cerr << "Couldn't open " + fullReportFilename + '\n';
		return 1;
	}
	std::string line;
	std::cout << reportFile << '\n';
	while (getline(finReport, line)) {
		std::cout << line << '\n';
	}
	finReport.close();

	return 0;
}
