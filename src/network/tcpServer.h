#pragma once

#include "tcpSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <string>

typedef void* (*ClientHandler)(void*);
const int BACKLOG = 10;

class TCPServer {
private:
    TCPSocket* _socket;
    addrinfo* _addr;
public:
    bool serve();
    void acceptConnections(ClientHandler handler);

    TCPServer(const std::string& port) {
        addrinfo hints = {0};

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, port.c_str(), &hints, &_addr);

        int socketFd = socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);

        _socket = new TCPSocket(socketFd);
    }
    ~TCPServer() {
        close(_socket->getSocketFd());
        freeaddrinfo(_addr);
        delete _socket;
    }
};