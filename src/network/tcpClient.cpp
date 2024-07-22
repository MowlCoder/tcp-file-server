#include "tcpClient.h"

#include <iostream>
#include <signal.h>

bool TCPClient::connectToServer() {
    #ifdef SIGPIPE
        signal(SIGPIPE, SIG_IGN);
    #endif

    _tcpSocket->initSocket();
    _reader = std::make_shared<NetworkReader>(_tcpSocket->getSocket());
    _writer = std::make_shared<NetworkWriter>(_tcpSocket->getSocket());

    if (connect(_tcpSocket->getSocketFd(), _tcpSocket->getAddr()->ai_addr, _tcpSocket->getAddr()->ai_addrlen) < 0) {
        #ifdef _WIN32
            std::cout << "ERROR: can not connect to server, error code - " << WSAGetLastError() << std::endl;
        #else
            std::cout << "ERROR: can not connect to server, error code: ";
            perror("connect");
        #endif
        return false;
    }

    return true;
}

const std::shared_ptr<NetworkReader> TCPClient::reader() const {
    return _reader;
}

const std::shared_ptr<NetworkWriter> TCPClient::writer() const {
    return _writer;
}