#pragma once

#include "tcpSocket.h"
#include "reader.h"
#include "writer.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

class TCPClient {
private:
    std::shared_ptr<TCPSocket> _tcpSocket;

    std::shared_ptr<NetworkReader> _reader;
    std::shared_ptr<NetworkWriter> _writer;
public:
    bool connectToServer();
    const std::shared_ptr<NetworkReader> reader() const;
    const std::shared_ptr<NetworkWriter> writer() const;

    TCPClient(const std::string& serverIp, const std::string& serverPort) {
        _tcpSocket = std::make_shared<TCPSocket>(serverIp, serverPort);
    }
    ~TCPClient() {
        _tcpSocket->closeSocket();
    }
};