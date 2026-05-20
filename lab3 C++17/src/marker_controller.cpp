// marker_controller.cpp
//
// lab3 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "marker_controller.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

std::mutex MarkerController::cout_mtx;

MarkerController::MarkerController(std::size_t array_size) {
    if (array_size == 0) {
        throw std::invalid_argument("array_size must be > 0");
    }
    array = std::make_shared<MarkerArray>(array_size);
}

MarkerController::~MarkerController() {
    join_all();
}

void MarkerController::sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void MarkerController::start_markers(std::size_t count) {
    if (count == 0) return;
    
    std::lock_guard<std::mutex> lock(ctrl_mtx);
    markers.clear();
    markers.reserve(count);
    
    for (std::size_t i = 0; i < count; ++i) {
        markers.push_back(std::make_unique<MarkerInfo>());
    }
    
    for (std::size_t i = 0; i < count; ++i) {
        markers[i]->thread = std::thread(&MarkerController::marker_worker, this, i + 1);
    }
}

void MarkerController::signal_start_all() {
    {
        std::lock_guard<std::mutex> lock(start_mtx);
        started.store(true);
    }
    start_cv.notify_all();
}

void MarkerController::marker_worker(std::size_t id) {
    std::size_t array_size = array->size();
    
    std::srand(static_cast<unsigned int>(id));
    
    {
        std::unique_lock<std::mutex> lock(start_mtx);
        start_cv.wait(lock, [this] { return started.load(); });
    }
    
    while (true) {
        {
            std::lock_guard<std::mutex> lock(ctrl_mtx);
            if (markers[id - 1]->terminate.load()) break;
        }
        
        std::size_t index = static_cast<std::size_t>(std::rand()) % array_size;
        
        if (array->get(index) == 0) {
            sleep_ms(5);
            if (array->try_mark(index, static_cast<int>(id))) {
                sleep_ms(5);
                continue;
            }
        }
        
        std::size_t marked_count = array->count_marked_by(static_cast<int>(id));
        
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Marker " << id 
                      << " | Marked: " << marked_count 
                      << " | Blocked at index: " << index << std::endl;
        }
        
        markers[id - 1]->waiting.store(true);
        ctrl_cv.notify_one();
        
        std::unique_lock<std::mutex> lock(markers[id - 1]->cv_mtx);
        markers[id - 1]->cv.wait(lock, [this, id] {
            return markers[id - 1]->terminate.load() || !markers[id - 1]->waiting.load();
        });
        
        if (markers[id - 1]->terminate.load()) break;
    }
    
    array->clear_marks(static_cast<int>(id));
    markers[id - 1]->alive.store(false);
    ctrl_cv.notify_one();
    
    {
        std::lock_guard<std::mutex> lock(cout_mtx);
        std::cout << "Marker " << id << " terminated and cleared its marks" << std::endl;
    }
}

void MarkerController::wait_all_blocked() {
    std::unique_lock<std::mutex> lock(ctrl_mtx);
    ctrl_cv.wait(lock, [this] {
        std::size_t active = 0;
        std::size_t waiting = 0;
        for (auto& marker : markers) {
            if (marker->alive.load()) {
                ++active;
                if (marker->waiting.load()) ++waiting;
            }
        }
        return active > 0 && waiting == active;
    });
}

bool MarkerController::terminate_marker(std::size_t marker_id) {
    if (marker_id == 0 || marker_id > markers.size()) return false;
    
    {
        std::lock_guard<std::mutex> lock(ctrl_mtx);
        if (!markers[marker_id - 1]->alive.load()) return false;
        markers[marker_id - 1]->terminate.store(true);
        markers[marker_id - 1]->cv.notify_one();
    }
    
    if (markers[marker_id - 1]->thread.joinable()) {
        markers[marker_id - 1]->thread.join();
    }
    
    return true;
}

void MarkerController::resume_all() {
    std::lock_guard<std::mutex> lock(ctrl_mtx);
    for (auto& marker : markers) {
        if (marker->waiting.load() && marker->alive.load() && !marker->terminate.load()) {
            marker->waiting.store(false);
            marker->cv.notify_one();
        }
    }
}

void MarkerController::print_array(std::ostream& os) const {
    std::vector<int> snapshot = array->copy();
    for (std::size_t i = 0; i < snapshot.size(); ++i) {
        os << snapshot[i];
        if (i + 1 < snapshot.size()) os << " ";
    }
    os << std::endl;
}

bool MarkerController::has_active_markers() const {
    for (auto& marker : markers) {
        if (marker->alive.load()) return true;
    }
    return false;
}

std::size_t MarkerController::alive_count() const {
    std::size_t count = 0;
    for (auto& marker : markers) {
        if (marker->alive.load()) ++count;
    }
    return count;
}

std::size_t MarkerController::find_first_alive() const {
    for (std::size_t i = 0; i < markers.size(); ++i) {
        if (markers[i]->alive.load()) return i + 1;
    }
    return 0;
}

void MarkerController::join_all() {
    std::lock_guard<std::mutex> lock(ctrl_mtx);
    for (auto& marker : markers) {
        if (!marker) continue;
        marker->terminate.store(true);
        marker->cv.notify_one();
    }
    for (auto& marker : markers) {
        if (marker && marker->thread.joinable()) {
            marker->thread.join();
        }
    }
    markers.clear();
}