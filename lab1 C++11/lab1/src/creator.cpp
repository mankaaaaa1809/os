#include <iostream>
#include <windows.h>
#include <fstream>
#include "employee.h"

int main(int argc, char* args[]) {
	if (argc != 2) {
		std::cerr << "Incorrect number of arguments\n";
		return 1;
	}

	const char* binaryFilename = args[0];
	const int count = std::atoi(args[1]);

	if (count <= 0) {
		std::cerr << "Count must be positive\n";
		return 1;
	}

	std::ofstream fout(binaryFilename, std::ios::binary);
	if (!fout) {
		std::cerr << "Couldn't open file\n";
		return 1;
	}

	for (int i = 0; i < count; i++) {
		employee e;
		std::cout << i + 1 << ": ";
		e.num = validateNum();
		strcpy(e.name, validateName().c_str());
		e.hours = validateHours();
		fout.write(reinterpret_cast<char*>(&e), sizeof(employee));
	}

	fout.close();

	return 0;
}
