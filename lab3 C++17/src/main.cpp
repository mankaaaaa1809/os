// main.cpp
//
// lab3 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "marker_controller.h"
#include <iostream>
#include <limits>
#include <string>

static int read_positive_int(const char* prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error: enter a valid number." << std::endl;
            continue;
        }
        if (value <= 0) {
            std::cerr << "Error: number must be > 0." << std::endl;
            continue;
        }
        return value;
    }
}

static int read_marker_id(int max_id) {
    int id = 0;
    while (true) {
        std::cout << "Enter marker id to terminate: ";
        if (!(std::cin >> id)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input! Please enter a number." << std::endl;
            continue;
        }
        if (id < 1 || id > max_id) {
            std::cerr << "Marker id must be in range [1.." << max_id << "]." << std::endl;
            continue;
        }
        return id;
    }
}

int main() {
    try {
        int size = read_positive_int("Enter array size: ");
        MarkerController controller(static_cast<std::size_t>(size));
        
        int count = read_positive_int("Enter number of marker threads: ");
        controller.start_markers(static_cast<std::size_t>(count));
        controller.signal_start_all();
        
        while (controller.has_active_markers()) {
            controller.wait_all_blocked();
            
            std::cout << "\nArray state BEFORE termination:" << std::endl;
            controller.print_array(std::cout);
            
            if (controller.alive_count() == 1) {
                int last = static_cast<int>(controller.find_first_alive());
                if (last != 0) {
                    std::cout << "Only one marker (" << last << ") remains - terminating it automatically." << std::endl;
                    controller.terminate_marker(static_cast<std::size_t>(last));
                    std::cout << "\nArray state AFTER termination:" << std::endl;
                    controller.print_array(std::cout);
                    break;
                }
            }
            
            int id = read_marker_id(count);
            
            if (!controller.terminate_marker(static_cast<std::size_t>(id))) {
                std::cerr << "Invalid or already terminated marker. Try another id." << std::endl;
                continue;
            }
            
            std::cout << "\nArray state AFTER termination:" << std::endl;
            controller.print_array(std::cout);
            
            if (!controller.has_active_markers()) break;
            
            controller.resume_all();
        }
        
        controller.join_all();
        std::cout << "\nAll marker threads finished." << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 2;
    }
}