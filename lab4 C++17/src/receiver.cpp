// receiver.cpp
//
// lab4 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "../include/file_queue.h"
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <cstdlib>

static unsigned int read_positive_uint(const std::string& prompt) {
    std::regex reg("^[1-9][0-9]*$");
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (!std::regex_match(input, reg)) {
            std::cout << "Invalid input. Positive integer required.\n";
            continue;
        }
        return static_cast<unsigned int>(std::stoul(input));
    }
}

static void start_senders(const std::string& file, unsigned int count) {
    for (unsigned int i = 0; i < count; ++i) {
        STARTUPINFOA si = {};
        PROCESS_INFORMATION pi = {};
        si.cb = sizeof(si);
        std::string cmd = "sender.exe " + file;
        std::vector<char> buf(cmd.begin(), cmd.end());
        buf.push_back('\0');
        CreateProcessA(nullptr, buf.data(), nullptr, nullptr, FALSE,
                       CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}

int main() {
    std::string filename;
    std::cout << "Enter binary file name: ";
    std::getline(std::cin, filename);

    unsigned int capacity = read_positive_uint("Enter number of records: ");
    unsigned int sender_count = read_positive_uint("Enter number of Sender processes: ");

    FileQueue queue(filename, capacity);
    start_senders(filename, sender_count);
    queue.wait_all_senders(sender_count);

    std::cout << "All senders ready. Commands: read / exit\n";

    while (true) {
        std::cout << "Command: ";
        std::string cmd;
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            queue.signal_shutdown();
            break;
        } else if (cmd == "read") {
            std::string msg = queue.pop();
            if (msg.empty() && queue.is_shutting_down())
                break;
            std::cout << "Received: " << msg << "\n";
        } else {
            std::cout << "Unknown command\n";
        }
    }
    return 0;
}