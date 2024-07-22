#include "tcpSocket.h"
#include "errors.h"

#include <exception>
#include <cerrno>

void TCPSocket::initSocket() {
    #ifdef _WIN32
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            throw std::runtime_error("WSAStartup() failed with error");
        }
    #endif

    addrinfo hints = {0};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int errorCode = getaddrinfo(_ip.c_str(), _port.c_str(), &hints, &_addr);
    if (errorCode != 0) {
        throw std::runtime_error("getaddrinfo failed with error " + std::to_string(errorCode));
    }

    int socketFd = socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);

    _socket = std::make_shared<Socket>(socketFd);
}

void TCPSocket::closeSocket() {
    #ifdef _WIN32
        WSACleanup();
        closesocket(_socket->getSocketFd());
    #else
        close(_socket->getSocketFd());
    #endif

    freeaddrinfo(_addr);
}

std::shared_ptr<Socket> TCPSocket::getSocket() const {
    return _socket;
}

const int TCPSocket::getSocketFd() const {
    return _socket->getSocketFd();
}

const addrinfo* TCPSocket::getAddr() const {
    return _addr;
}