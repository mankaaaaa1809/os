#include <iostream>
#include <string>
#include "employee.h"
#include "regex"

double countWage(double workHours, double payRate) {
	return workHours * payRate;
}

int validateNum() {
	std::string numStr;
	std::regex pattern("^[1-9][0-9]*$");
	std::cout << "Enter a number of the employee: ";
	while (true) {
		std::cin >> numStr;
		if (std::regex_match(numStr, pattern)) {
			return std::stoi(numStr);
		}
		else {
			std::cout << "Wrong format, try again\n";
		}
	}
}
std::string validateName() {
	std::string name;
	std::regex pattern("^[A-Z][a-z]{0,8}$");
	std::cout << "Enter a name: ";
	while (true) {
		std::cin >> name;
		if (std::regex_match(name, pattern)) {
			return name;
		}
		else {
			std::cout << "Wrong format, try again\n";
		}
	}

}
double validateHours() {
	std::string timeStr;
	std::regex pattern("^[0-9]+(\\.[0-9]+)?$");
	std::cout << "Enter hours: ";
	while (true) {
		std::cin >> timeStr;
		if (std::regex_match(timeStr, pattern)) {
			return std::stod(timeStr);
		}
		else {
			std::cout << "Wrong format, try again\n";
		}
	}
}