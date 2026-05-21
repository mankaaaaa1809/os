// request.h
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#ifndef REQUEST_H
#define REQUEST_H

#include "employee.h"
#include <windows.h>
#include <string>

static const char* PIPE_NAME = "\\\\.\\pipe\\EmployeePipe";

enum RequestType {
    REQ_READ = 0,
    REQ_MODIFY_START = 1,
    REQ_MODIFY_COMMIT = 2,
    REQ_EXIT = 3
};

struct Request {
    RequestType type;
    int emp_id;
    Employee data;

    Request();
};

struct Response {
    bool success;
    Employee data;
    std::string message;

    Response();
};

bool SendRequest(HANDLE hPipe, const Request& req);
bool ReadRequest(HANDLE hPipe, Request& req);
bool SendResponse(HANDLE hPipe, const Response& resp);
bool ReadResponse(HANDLE hPipe, Response& resp);

#endif