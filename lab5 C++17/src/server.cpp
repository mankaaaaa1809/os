// server.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "request.h"
#include "file_handler.h"
#include "readers_writer.h"
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <chrono>

struct ClientHandlerParams {
    HANDLE hPipe;
    FileHandler* fileHandler;
    RecordLocks* recordLocks;
    std::vector<Employee>* employees;
    std::atomic<int>* activeClients;
};

static int findEmployeeIndex(const std::vector<Employee>& employees, int id) {
    for (size_t i = 0; i < employees.size(); ++i) {
        if (employees[i].num == id) return static_cast<int>(i);
    }
    return -1;
}

void clientHandler(ClientHandlerParams* params) {
    HANDLE hPipe = params->hPipe;
    FileHandler* fh = params->fileHandler;
    RecordLocks* locks = params->recordLocks;
    std::vector<Employee>* employees = params->employees;
    std::atomic<int>* activeClients = params->activeClients;

    while (true) {
        Request req;
        if (!ReadRequest(hPipe, req)) break;

        Response resp;
        resp.success = false;

        if (req.type == REQ_EXIT) {
            resp.success = true;
            resp.message = "Goodbye";
            SendResponse(hPipe, resp);
            break;
        }

        int index = findEmployeeIndex(*employees, req.emp_id);

        if (index == -1) {
            resp.success = false;
            resp.message = "Employee not found";
            SendResponse(hPipe, resp);
            continue;
        }

        if (req.type == REQ_READ) {
            locks->lock_read(index);
            Employee e;
            if (fh->GetEmployee(req.emp_id, e)) {
                resp.success = true;
                resp.data = e;
                resp.message = "Read successful";
                SendResponse(hPipe, resp);
            } else {
                resp.message = "Read failed";
                SendResponse(hPipe, resp);
            }
            locks->unlock_read(index);
        }
        else if (req.type == REQ_MODIFY_START) {
            locks->lock_write(index);
            Employee e;
            if (fh->GetEmployee(req.emp_id, e)) {
                resp.success = true;
                resp.data = e;
                resp.message = "Record locked for modification";
                SendResponse(hPipe, resp);
            } else {
                resp.message = "Record not found";
                SendResponse(hPipe, resp);
                locks->unlock_write(index);
            }
        }
        else if (req.type == REQ_MODIFY_COMMIT) {
            Employee oldEmp;
            if (fh->GetEmployee(req.data.num, oldEmp)) {
                if (fh->UpdateEmployee(req.data)) {
                    for (auto& e : *employees) {
                        if (e.num == req.data.num) {
                            e = req.data;
                            break;
                        }
                    }
                    resp.success = true;
                    resp.message = "Modification committed";
                } else {
                    resp.message = "Commit failed";
                }
            } else {
                resp.message = "Original record not found";
            }
            SendResponse(hPipe, resp);
            locks->unlock_write(index);
        }
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    (*activeClients)--;
    delete params;
}

int main() {
    std::string filename;
    int employee_count;
    int client_count;

    std::cout << "Enter filename: ";
    std::cin >> filename;

    std::cout << "Enter number of employees: ";
    std::cin >> employee_count;

    std::vector<Employee> employees;
    for (int i = 0; i < employee_count; ++i) {
        int id;
        std::string name;
        double hours;
        std::cout << "Employee " << (i + 1) << " ID: ";
        std::cin >> id;
        std::cout << "Employee " << (i + 1) << " Name: ";
        std::cin >> name;
        std::cout << "Employee " << (i + 1) << " Hours: ";
        std::cin >> hours;
        employees.push_back(Employee(id, name, hours));
    }

    FileHandler fh(filename);
    fh.CreateFileWithData(employees);
    fh.ReadAllAndPrint();

    std::cout << "Enter number of client processes: ";
    std::cin >> client_count;

    RecordLocks recordLocks(employee_count);
    std::atomic<int> activeClients(client_count);

    for (int i = 0; i < client_count; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        char cmdLine[] = "client.exe";
        if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE,
                           CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cout << "Failed to create client process." << std::endl;
            activeClients--;
        } else {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    std::cout << "Server listening on pipe..." << std::endl;

    for (int i = 0; i < client_count; ++i) {
        HANDLE hPipe = CreateNamedPipe(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            4096, 4096, 0, NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cout << "Failed to create pipe." << std::endl;
            continue;
        }

        if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
            std::cout << "Client connected." << std::endl;
            ClientHandlerParams* params = new ClientHandlerParams;
            params->hPipe = hPipe;
            params->fileHandler = &fh;
            params->recordLocks = &recordLocks;
            params->employees = &employees;
            params->activeClients = &activeClients;
            std::thread t(clientHandler, params);
            t.detach();
        } else {
            CloseHandle(hPipe);
        }
    }

    while (activeClients.load() > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "All clients finished." << std::endl;
    fh.ReadAllAndPrint();

    std::cout << "Press any key to exit server...";
    std::string dummy;
    std::cin >> dummy;

    return 0;
}