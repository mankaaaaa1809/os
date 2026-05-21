// client.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "request.h"
#include <windows.h>
#include <iostream>
#include <conio.h>

int main() {
    std::cout << "Client: Connecting to server..." << std::endl;

    HANDLE hPipe = INVALID_HANDLE_VALUE;
    while (hPipe == INVALID_HANDLE_VALUE) {
        WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER);
        hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, 0, NULL
        );
    }

    std::cout << "Client: Connected." << std::endl;

    DWORD mode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

    bool running = true;

    while (running) {
        std::cout << "\n1. Read\n2. Modify\n3. Exit\nChoice: ";
        int choice;
        std::cin >> choice;

        Request req;
        Response resp;

        if (choice == 3) {
            req.type = REQ_EXIT;
            SendRequest(hPipe, req);
            running = false;
            continue;
        }

        std::cout << "Enter Employee ID: ";
        std::cin >> req.emp_id;

        if (choice == 1) {
            req.type = REQ_READ;
            if (!SendRequest(hPipe, req)) break;
            if (!ReadResponse(hPipe, resp)) break;

            if (resp.success) {
                resp.data.Print();
                std::cout << "Press any key to finish reading...";
                _getch();
                std::cout << std::endl;
            } else {
                std::cout << "Server: " << resp.message << std::endl;
            }
        }
        else if (choice == 2) {
            req.type = REQ_MODIFY_START;
            if (!SendRequest(hPipe, req)) break;
            if (!ReadResponse(hPipe, resp)) break;

            if (resp.success) {
                std::cout << "Current record:" << std::endl;
                resp.data.Print();
                std::cout << "Enter new Name: ";
                std::cin >> req.data.name;
                std::cout << "Enter new Hours: ";
                std::cin >> req.data.hours;
                req.data.num = req.emp_id;
                req.type = REQ_MODIFY_COMMIT;

                std::cout << "Sending update..." << std::endl;
                SendRequest(hPipe, req);
                if (!ReadResponse(hPipe, resp)) break;
                std::cout << "Server: " << resp.message << std::endl;

                std::cout << "Press any key to release lock...";
                _getch();
                std::cout << std::endl;
            } else {
                std::cout << "Server: " << resp.message << std::endl;
            }
        }
    }

    CloseHandle(hPipe);
    return 0;
}