// marker_controller.h
//
// lab3 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#pragma once
#include "marker_array.h"
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <ostream>

class MarkerController {
public:
    explicit MarkerController(std::size_t array_size);
    ~MarkerController();

    void start_markers(std::size_t count);
    void signal_start_all();
    void wait_all_blocked();
    bool terminate_marker(std::size_t marker_id);
    void resume_all();
    void print_array(std::ostream& os) const;
    bool has_active_markers() const;
    std::size_t alive_count() const;
    std::size_t find_first_alive() const;
    void join_all();

private:
    void marker_worker(std::size_t id);
    void sleep_ms(int ms);

    std::shared_ptr<MarkerArray> array;

    struct MarkerInfo {
        std::thread thread;
        std::atomic<bool> waiting{ false };
        std::atomic<bool> terminate{ false };
        std::atomic<bool> alive{ true };
        std::mutex cv_mtx;
        std::condition_variable cv;
    };
    
    std::vector<std::unique_ptr<MarkerInfo>> markers;
    mutable std::mutex ctrl_mtx;
    std::condition_variable ctrl_cv;
    std::atomic<bool> started{ false };
    std::mutex start_mtx;
    std::condition_variable start_cv;
    static std::mutex cout_mtx;
};