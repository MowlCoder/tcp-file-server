#include "tcpClient.h"

#include <signal.h>

bool TCPClient::connectToServer() {
    signal(SIGPIPE, SIG_IGN);

    if (connect(_socket->getSocketFd(), _addr->ai_addr, _addr->ai_addrlen) < 0) {
        fprintf(stderr, "ERROR: can not connect to server\n");
        return false;
    }

    return true;
}

const NetworkReader* TCPClient::reader() const {
    return _reader;
}

const NetworkWriter* TCPClient::writer() const {
    return _writer;
}