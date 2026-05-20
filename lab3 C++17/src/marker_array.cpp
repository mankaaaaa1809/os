// marker_array.cpp
//
// lab3 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "marker_array.h"
#include <algorithm>

MarkerArray::MarkerArray(std::size_t size) : data(size, 0) {}

bool MarkerArray::try_mark(std::size_t index, int marker_id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (index >= data.size()) return false;
    if (data[index] == 0) {
        data[index] = marker_id;
        return true;
    }
    return false;
}

void MarkerArray::clear_marks(int marker_id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& value : data) {
        if (value == marker_id) value = 0;
    }
}

std::vector<int> MarkerArray::copy() const {
    std::lock_guard<std::mutex> lock(mtx);
    return data;
}

std::size_t MarkerArray::size() const noexcept {
    return data.size();
}

std::size_t MarkerArray::count_marked_by(int marker_id) const {
    std::lock_guard<std::mutex> lock(mtx);
    return std::count(data.begin(), data.end(), marker_id);
}

int MarkerArray::get(std::size_t index) const {
    std::lock_guard<std::mutex> lock(mtx);
    if (index >= data.size()) return -1;
    return data[index];
}