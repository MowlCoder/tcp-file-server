#pragma once

#include <cstdlib>

class Socket {
private:
    int _socketFd;
public:
    int sendData(void* buf, size_t size);
    int recvData(void* buf, size_t size);

    void initWinsock();

    int getSocketFd() const;

    Socket(const int socketFd) : _socketFd(socketFd) {}
};