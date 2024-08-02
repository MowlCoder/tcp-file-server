#include "socket.h"

#include "errors.h"

#include <exception>
#include <cstring>
#include <cerrno>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#else
    #include <sys/socket.h>
#endif

int Socket::sendData(void* buf, size_t size) {
    int bytesSend = send(_socketFd, (const char *)buf, size, 0);

    if (bytesSend < 0) {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN || errno == 0) {
            throw SocketClosedException(_socketFd);
        }

        throw std::runtime_error("socket sendData unhandled error with code: " + std::string(strerror(errno)));
    }

    return bytesSend;
}

int Socket::recvData(void* buf, size_t size) {
    int bytesRecv = recv(_socketFd, (char *)buf, size, 0);

    if (bytesRecv < 0) {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN || errno == 0) {
            throw SocketClosedException(_socketFd);
        }

        throw std::runtime_error("socket recvData unhandled error with code: " + std::string(strerror(errno)));
    }

    return bytesRecv;
}

int Socket::getSocketFd() const {
    return _socketFd;
}

void Socket::close() {
    #ifdef _WIN32
        WSACleanup();
        closesocket(_socketFd);
    #else
        close(_socketFd);
    #endif
}