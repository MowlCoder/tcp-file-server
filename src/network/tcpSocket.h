#pragma once

#include <cstdlib>

class TCPSocket {
private:
    int _socketFd;
public:
    int sendData(void* buf, size_t size);
    int recvData(void* buf, size_t size);

    int getSocketFd() const;

    TCPSocket(const int socketFd) : _socketFd(socketFd) {}
};