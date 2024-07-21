#pragma once

#include "tcpSocket.h"
#include "reader.h"
#include "writer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <string>

class TCPClient {
private:
    TCPSocket* _socket;
    addrinfo* _addr;
    
    NetworkReader* _reader;
    NetworkWriter* _writer;
public:
    bool connectToServer();
    const NetworkReader* reader() const;
    const NetworkWriter* writer() const;

    TCPClient(const std::string& serverPort) {
        addrinfo hints = {0};

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, serverPort.c_str(), &hints, &_addr);

        int socketFd = socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);

        _socket = new TCPSocket(socketFd);
        _reader = new NetworkReader(_socket);
        _writer = new NetworkWriter(_socket);
    }
    ~TCPClient() {
        close(_socket->getSocketFd());
        freeaddrinfo(_addr);

        delete _reader;
        delete _writer;
        delete _socket;
    }
};