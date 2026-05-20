#include "../include/averageCalculator.h"
#include <numeric>

averageResult globalAverageResult;

DWORD WINAPI computeAverage(LPVOID param) {
    auto* data = static_cast<std::vector<long long>*>(param);
    if (!data || data->empty()) {
        return 1;
    }

    long long total = 0;
    for (int val : *data) {
        total += val;
        Sleep(12);
    }

    double averageVal = static_cast<double>(total) / data->size();
    globalAverageResult = { averageVal };

    std::lock_guard lock(consoleMutex);
    std::cout << "Average = " << averageVal << "\n";
    return 0;
}