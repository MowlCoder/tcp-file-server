#pragma once

#include "tcpSocket.h"

#ifdef _WIN32
    #include <thread>
#endif

#include <iostream>
#include <exception>
#include <string>
#include <memory>

typedef void* (*ClientHandler)(void*);
const int BACKLOG = 10;

class TCPServer {
private:
    std::shared_ptr<TCPSocket> _socket;
public:
    bool serve();
    void acceptConnections(ClientHandler handler);

    TCPServer(const std::string& port) {
        _socket = std::make_shared<TCPSocket>("localhost", port);
    }
    ~TCPServer() {
        _socket->closeSocket();
    }
};