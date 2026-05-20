#include "../include/rangeFinder.h"

RangeResult globalMinMaxResult;
std::mutex consoleMutex;

DWORD WINAPI findRange(LPVOID param) {
    auto* data = static_cast<std::vector<long long>*>(param);
    if (!data || data->empty()) {
        return 1;
    }

    long long smallest = (*data)[0];
    long long largest = (*data)[0];

    for (size_t i = 1; i < data->size(); ++i) {
        smallest = (std::min)(smallest, (*data)[i]);
        largest = (std::max)(largest, (*data)[i]);
        Sleep(7);
    }

    globalMinMaxResult = { smallest, largest };

    std::lock_guard lock(consoleMutex);
    std::cout << "Min = " << smallest << "\n";
    std::cout << "Max = " << largest << "\n";

    return 0;
}