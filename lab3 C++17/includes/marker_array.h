// marker_array.h
//
// lab3 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#pragma once
#include <vector>
#include <mutex>

class MarkerArray {
public:
    explicit MarkerArray(std::size_t size);
    ~MarkerArray() = default;

    bool try_mark(std::size_t index, int marker_id);
    void clear_marks(int marker_id);
    std::vector<int> copy() const;
    std::size_t size() const noexcept;
    std::size_t count_marked_by(int marker_id) const;
    int get(std::size_t index) const;

private:
    std::vector<int> data;
    mutable std::mutex mtx;
};