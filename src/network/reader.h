#pragma once

#include "tcpSocket.h"

#include <string>
#include <vector>

class NetworkReader {
private:
    TCPSocket* _tcpSocket;
public:
    std::string ReadString() const;
    int32_t ReadInt32() const;
    bool ReadBool() const;

    std::vector<std::string> ReadStringList() const;
    void ReadFile(const std::string& defaultPath) const;
    NetworkReader(TCPSocket* tcpSocket);
};