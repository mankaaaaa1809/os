#include "../include/dataInput.h"
#include "../include/rangeFinder.h"
#include "../include/averageCalculator.h"

#include <windows.h>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>

int main() {
    try {
        auto numbers = readConsoleData();

        HANDLE hMinMax = CreateThread(nullptr, 0, findRange, &numbers, 0, nullptr);
        HANDLE hAverage = CreateThread(nullptr, 0, computeAverage, &numbers, 0, nullptr);

        if (!hMinMax || !hAverage)
            throw std::runtime_error("Failed to create threads");

        WaitForSingleObject(hMinMax, INFINITE);
        WaitForSingleObject(hAverage, INFINITE);

        CloseHandle(hMinMax);
        CloseHandle(hAverage);

        std::cout << "\nAll threads completed.\n";

        for (auto& val : numbers) {
            if (val == globalMinMaxResult.smallest || val == globalMinMaxResult.largest)
                val = std::round(globalAverageResult.value);
        }

        std::cout << "Array after replacement: ";
        showArray(numbers);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}