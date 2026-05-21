#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <mutex>

struct RangeResult {
    long long smallest = 0;
    long long largest = 0;
};

DWORD WINAPI findRange(LPVOID param);
extern RangeResult globalMinMaxResult;
extern std::mutex consoleMutex;