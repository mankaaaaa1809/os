// file_queue.cpp
//
// lab4 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "../include/file_queue.h"
#include <stdexcept>
#include <cstring>

FileQueue::FileQueue(const std::string& file_name, unsigned int capacity)
    : file_(INVALID_HANDLE_VALUE), file_name_(file_name), capacity_(capacity) {
    file_ = CreateFileA(file_name.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_ == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to create file");

    LARGE_INTEGER size;
    size.QuadPart = sizeof(QueueHeader) + capacity * sizeof(QueueSlot);
    SetFilePointerEx(file_, size, nullptr, FILE_BEGIN);
    SetEndOfFile(file_);

    QueueHeader header = {};
    header.capacity = capacity;
    header.head = 0;
    header.tail = 0;
    header.count = 0;
    header.ready_senders = 0;
    header.expected_senders = 0;
    header.shutting_down = 0;
    write_header(header);

    mutex_ = CreateMutexA(nullptr, FALSE, make_global_name("mutex").c_str());
    sem_empty_ = CreateSemaphoreA(nullptr, capacity, capacity, make_global_name("empty").c_str());
    sem_full_ = CreateSemaphoreA(nullptr, 0, capacity, make_global_name("full").c_str());
    ready_event_ = CreateEventA(nullptr, TRUE, FALSE, make_global_name("ready").c_str());

    if (!mutex_ || !sem_empty_ || !sem_full_ || !ready_event_)
        throw std::runtime_error("Failed to create sync objects");
}

FileQueue::FileQueue(const std::string& file_name)
    : file_(INVALID_HANDLE_VALUE), file_name_(file_name), capacity_(0) {
    file_ = CreateFileA(file_name.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_ == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to open file");

    QueueHeader header;
    read_header(header);
    capacity_ = header.capacity;

    mutex_ = OpenMutexA(SYNCHRONIZE, FALSE, make_global_name("mutex").c_str());
    sem_empty_ = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, make_global_name("empty").c_str());
    sem_full_ = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, make_global_name("full").c_str());
    ready_event_ = OpenEventA(EVENT_ALL_ACCESS, FALSE, make_global_name("ready").c_str());

    if (!mutex_ || !sem_empty_ || !sem_full_ || !ready_event_)
        throw std::runtime_error("Failed to open sync objects");
}

FileQueue::~FileQueue() {
    if (file_ != INVALID_HANDLE_VALUE) CloseHandle(file_);
    if (mutex_) CloseHandle(mutex_);
    if (sem_empty_) CloseHandle(sem_empty_);
    if (sem_full_) CloseHandle(sem_full_);
    if (ready_event_) CloseHandle(ready_event_);
}

std::string FileQueue::make_global_name(const std::string& suffix) {
    std::string base;
    for (char c : file_name_) {
        if (c == '\\' || c == '/' || c == ':' || c == '.')
            base += '_';
        else
            base += c;
    }
    return "Global\\fq_" + base + "_" + suffix;
}

void FileQueue::lock() { WaitForSingleObject(mutex_, INFINITE); }
void FileQueue::unlock() { ReleaseMutex(mutex_); }

void FileQueue::read_header(QueueHeader& header) {
    DWORD read;
    SetFilePointer(file_, 0, nullptr, FILE_BEGIN);
    ReadFile(file_, &header, sizeof(header), &read, nullptr);
}

void FileQueue::write_header(const QueueHeader& header) {
    DWORD written;
    SetFilePointer(file_, 0, nullptr, FILE_BEGIN);
    WriteFile(file_, &header, sizeof(header), &written, nullptr);
}

void FileQueue::write_slot(unsigned int index, const QueueSlot& slot) {
    DWORD written;
    SetFilePointer(file_, sizeof(QueueHeader) + index * sizeof(QueueSlot), nullptr, FILE_BEGIN);
    WriteFile(file_, &slot, sizeof(slot), &written, nullptr);
}

FileQueue::QueueSlot FileQueue::read_slot(unsigned int index) {
    QueueSlot slot = {};
    DWORD read;
    SetFilePointer(file_, sizeof(QueueHeader) + index * sizeof(QueueSlot), nullptr, FILE_BEGIN);
    ReadFile(file_, &slot, sizeof(slot), &read, nullptr);
    return slot;
}

void FileQueue::push(const std::string& message) {
    if (message.empty())
        throw std::runtime_error("Empty message");
    if (message.size() > 20)
        throw std::runtime_error("Message too long");

    WaitForSingleObject(sem_empty_, INFINITE);
    lock();

    QueueHeader header;
    read_header(header);
    if (header.shutting_down) {
        unlock();
        ReleaseSemaphore(sem_empty_, 1, nullptr);
        throw std::runtime_error("Shutting down");
    }

    QueueSlot slot = {};
    strncpy_s(slot.text, message.c_str(), 20);
    write_slot(header.tail, slot);

    header.tail = (header.tail + 1) % header.capacity;
    header.count++;
    write_header(header);

    unlock();
    ReleaseSemaphore(sem_full_, 1, nullptr);
}

std::string FileQueue::pop() {
    WaitForSingleObject(sem_full_, INFINITE);
    lock();

    QueueHeader header;
    read_header(header);
    if (header.shutting_down && header.count == 0) {
        unlock();
        return "";
    }

    QueueSlot slot = read_slot(header.head);
    header.head = (header.head + 1) % header.capacity;
    header.count--;
    write_header(header);

    unlock();
    ReleaseSemaphore(sem_empty_, 1, nullptr);
    return std::string(slot.text);
}

void FileQueue::signal_sender_ready() {
    lock();
    QueueHeader header;
    read_header(header);
    header.ready_senders++;
    write_header(header);
    if (header.ready_senders == header.expected_senders)
        SetEvent(ready_event_);
    unlock();
}

void FileQueue::wait_all_senders(unsigned int expected) {
    lock();
    QueueHeader header;
    read_header(header);
    header.expected_senders = expected;
    write_header(header);
    unlock();
    WaitForSingleObject(ready_event_, INFINITE);
}

void FileQueue::signal_shutdown() {
    lock();
    QueueHeader header;
    read_header(header);
    header.shutting_down = 1;
    write_header(header);
    unlock();

    ReleaseSemaphore(sem_full_, capacity_, nullptr);
    ReleaseSemaphore(sem_empty_, capacity_, nullptr);
    SetEvent(ready_event_);
}

bool FileQueue::is_shutting_down() const {
    QueueHeader header;
    const_cast<FileQueue*>(this)->read_header(header);
    return header.shutting_down != 0;
}