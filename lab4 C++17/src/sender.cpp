// sender.cpp
//
// lab4 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "file_queue.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: sender.exe <file_name>\n";
        return 1;
    }

    try {
        FileQueue queue(argv[1]);
        queue.signal_sender_ready();

        std::cout << "Sender ready. Commands: send / exit\n";

        while (true) {
            std::cout << "Command: ";
            std::string cmd;
            std::getline(std::cin, cmd);
            if (cmd == "exit") {
                break;
            } else if (cmd == "send") {
                std::cout << "Message (max 20 chars): ";
                std::string msg;
                std::getline(std::cin, msg);
                try {
                    queue.push(msg);
                    std::cout << "Sent\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Unknown command\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Sender error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}