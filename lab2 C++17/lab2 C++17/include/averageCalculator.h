#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <mutex>

struct averageResult {
    double value = 0.0;
};

DWORD WINAPI computeAverage(LPVOID param);
extern averageResult globalAverageResult;
extern std::mutex consoleMutex;