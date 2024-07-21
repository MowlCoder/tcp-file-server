#pragma once

#include <string>
#include <exception>
#include <sstream>

class SocketClosedException : public std::exception {
private:
    int32_t _socketFd;
public:
    virtual const char* what() const noexcept override {
        return "connection was closed";
    }

    SocketClosedException(int32_t socketFd) : _socketFd(socketFd) {}
};