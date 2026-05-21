// request.cpp
//
// lab5 C++17
//
// created by Anhelina Mankewich, 2 course, group 12

#include "request.h"
#include <vector>
#include <cstring>

static void append_bytes(std::vector<char>& buf, const void* src, size_t len) {
    const char* p = static_cast<const char*>(src);
    buf.insert(buf.end(), p, p + len);
}

template<typename T>
static T read_pod(const std::vector<char>& buf, size_t& pos) {
    T val;
    std::memcpy(&val, &buf[pos], sizeof(T));
    pos += sizeof(T);
    return val;
}

Request::Request() : type(REQ_READ), emp_id(0), data() {}
Response::Response() : success(false), data(), message() {}

static void serializeEmployee(const Employee& e, std::vector<char>& buf) {
    append_bytes(buf, &e.num, sizeof(int));
    unsigned int name_len = static_cast<unsigned int>(e.name.size());
    append_bytes(buf, &name_len, sizeof(unsigned int));
    if (name_len > 0) append_bytes(buf, e.name.c_str(), name_len);
    append_bytes(buf, &e.hours, sizeof(double));
}

static Employee deserializeEmployee(const std::vector<char>& buf, size_t& pos) {
    Employee e;
    e.num = read_pod<int>(buf, pos);
    unsigned int name_len = read_pod<unsigned int>(buf, pos);
    if (name_len > 0) {
        e.name.assign(&buf[pos], name_len);
        pos += name_len;
    } else {
        e.name = "";
    }
    e.hours = read_pod<double>(buf, pos);
    return e;
}

static std::vector<char> serializeRequest(const Request& req) {
    std::vector<char> buf;
    int type_int = static_cast<int>(req.type);
    append_bytes(buf, &type_int, sizeof(int));
    append_bytes(buf, &req.emp_id, sizeof(int));
    serializeEmployee(req.data, buf);
    return buf;
}

static Request deserializeRequest(const std::vector<char>& buf) {
    Request req;
    size_t pos = 0;
    int type_int = read_pod<int>(buf, pos);
    req.type = static_cast<RequestType>(type_int);
    req.emp_id = read_pod<int>(buf, pos);
    req.data = deserializeEmployee(buf, pos);
    return req;
}

static std::vector<char> serializeResponse(const Response& resp) {
    std::vector<char> buf;
    char succ = resp.success ? 1 : 0;
    append_bytes(buf, &succ, sizeof(char));
    serializeEmployee(resp.data, buf);
    unsigned int msg_len = static_cast<unsigned int>(resp.message.size());
    append_bytes(buf, &msg_len, sizeof(unsigned int));
    if (msg_len > 0) append_bytes(buf, resp.message.c_str(), msg_len);
    return buf;
}

static Response deserializeResponse(const std::vector<char>& buf) {
    Response resp;
    size_t pos = 0;
    char succ = read_pod<char>(buf, pos);
    resp.success = (succ != 0);
    resp.data = deserializeEmployee(buf, pos);
    unsigned int msg_len = read_pod<unsigned int>(buf, pos);
    if (msg_len > 0) {
        resp.message.assign(&buf[pos], msg_len);
        pos += msg_len;
    } else {
        resp.message = "";
    }
    return resp;
}

static bool send_buffer(HANDLE hPipe, const std::vector<char>& buf) {
    unsigned int sz = static_cast<unsigned int>(buf.size());
    DWORD written = 0;
    if (!WriteFile(hPipe, &sz, sizeof(unsigned int), &written, NULL)) return false;
    if (written != sizeof(unsigned int)) return false;
    if (sz == 0) return true;
    if (!WriteFile(hPipe, &buf[0], sz, &written, NULL)) return false;
    if (static_cast<unsigned int>(written) != sz) return false;
    return true;
}

static bool read_buffer(HANDLE hPipe, std::vector<char>& buf) {
    unsigned int sz = 0;
    DWORD read = 0;
    if (!ReadFile(hPipe, &sz, sizeof(unsigned int), &read, NULL)) return false;
    if (read != sizeof(unsigned int)) return false;
    buf.clear();
    if (sz == 0) return true;
    buf.resize(sz);
    DWORD totalRead = 0;
    while (totalRead < sz) {
        DWORD chunk = 0;
        if (!ReadFile(hPipe, &buf[totalRead], sz - totalRead, &chunk, NULL)) return false;
        if (chunk == 0) break;
        totalRead += chunk;
    }
    return totalRead == sz;
}

bool SendRequest(HANDLE hPipe, const Request& req) {
    std::vector<char> buf = serializeRequest(req);
    return send_buffer(hPipe, buf);
}

bool ReadRequest(HANDLE hPipe, Request& req) {
    std::vector<char> buf;
    if (!read_buffer(hPipe, buf)) return false;
    req = deserializeRequest(buf);
    return true;
}

bool SendResponse(HANDLE hPipe, const Response& resp) {
    std::vector<char> buf = serializeResponse(resp);
    return send_buffer(hPipe, buf);
}

bool ReadResponse(HANDLE hPipe, Response& resp) {
    std::vector<char> buf;
    if (!read_buffer(hPipe, buf)) return false;
    resp = deserializeResponse(buf);
    return true;
}