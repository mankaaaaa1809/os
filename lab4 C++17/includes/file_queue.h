// file_queue.h
//
// lab4 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#pragma once

#include <string>
#include <windows.h>

class FileQueue {
public:
    FileQueue(const std::string& file_name, unsigned int capacity);
    FileQueue(const std::string& file_name);
    ~FileQueue();

    void push(const std::string& message);
    std::string pop();

    void signal_sender_ready();
    void wait_all_senders(unsigned int expected);
    void signal_shutdown();
    bool is_shutting_down() const;

private:
    struct QueueHeader {
        unsigned int capacity;
        unsigned int head;
        unsigned int tail;
        unsigned int count;
        unsigned int ready_senders;
        unsigned int expected_senders;
        unsigned char shutting_down;
    };

    struct QueueSlot {
        char text[21];
    };

    HANDLE file_;
    HANDLE mutex_;
    HANDLE sem_empty_;
    HANDLE sem_full_;
    HANDLE ready_event_;
    std::string file_name_;
    unsigned int capacity_;

    void read_header(QueueHeader& header);
    void write_header(const QueueHeader& header);
    void write_slot(unsigned int index, const QueueSlot& slot);
    QueueSlot read_slot(unsigned int index);
    void lock();
    void unlock();
    std::string make_global_name(const std::string& suffix);
};