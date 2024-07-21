#include "tcpSocket.h"

#include "errors.h"

#include <exception>
#include <sys/socket.h>
#include <cerrno>

int TCPSocket::sendData(void* buf, size_t size) {
    int bytesSend = send(_socketFd, buf, size, 0);

    if (bytesSend < 0) {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN) {
            throw SocketClosedException(_socketFd);
        }

        throw std::runtime_error("unhandled error with code " + errno);
    }

    return bytesSend;
}

int TCPSocket::recvData(void* buf, size_t size) {
    int bytesRecv = recv(_socketFd, buf, size, 0);

    if (bytesRecv < 0) {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN) {
            throw SocketClosedException(_socketFd);
        }

        throw std::runtime_error("unhandled error with code " + errno);
    }

    return bytesRecv;
}

int TCPSocket::getSocketFd() const {
    return _socketFd;
}