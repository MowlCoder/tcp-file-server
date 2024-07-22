#pragma once

#include "socket.h"

#include <string>
#include <vector>
#include <memory>

class NetworkReader {
private:
    std::shared_ptr<Socket> _socket;
public:
    std::string ReadString() const;
    int32_t ReadInt32() const;
    bool ReadBool() const;

    std::vector<std::string> ReadStringList() const;
    void ReadFile(const std::string& defaultPath) const;

    NetworkReader(std::shared_ptr<Socket> socket) : _socket(socket) {}
};