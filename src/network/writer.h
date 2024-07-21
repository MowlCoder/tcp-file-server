#pragma once

#include <string>
#include <vector>

#include "tcpSocket.h"

class NetworkWriter {
private:
    TCPSocket* _tcpSocket;
public:
    void WriteString(const std::string& val) const;
    void WriteInt32(int32_t val) const;
    void WriteBool(bool val) const;

    void WriteStringList(const std::vector<std::string>& list) const;
    void WriteFile(const std::string& path, const std::string& filename) const;
    NetworkWriter(TCPSocket* tcpSocket);
};