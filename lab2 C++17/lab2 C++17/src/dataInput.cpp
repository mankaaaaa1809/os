#include "../include/dataInput.h"

std::vector<long long> readConsoleData() {
    int count;
    std::cout << "Enter number of elements: ";
    if (!(std::cin >> count) || count <= 0) {
        throw std::invalid_argument("Invalid array size");
    }

    std::vector < long long > numbers(count);
    std::cout << "Enter " << count << " integers:\n";
    for (auto& val : numbers) {
        if (!(std::cin >> val))
            throw std::invalid_argument("Invalid input");
    }
    return numbers;
}

void showArray(const std::vector<long long>& data) {
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}