#pragma once

#include <string>
#include <vector>
#include <memory>

#include "socket.h"

class NetworkWriter {
private:
    std::shared_ptr<Socket> _socket;
public:
    void WriteString(const std::string& val) const;
    void WriteInt32(int32_t val) const;
    void WriteBool(bool val) const;

    void WriteStringList(const std::vector<std::string>& list) const;
    void WriteFile(const std::string& path, const std::string& filename) const;

    NetworkWriter(std::shared_ptr<Socket> socket) : _socket(socket) {}
};