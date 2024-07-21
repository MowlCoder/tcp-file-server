#include "reader.h"
#include "errors.h"

#include <arpa/inet.h>
#include <sys/socket.h>

#include <fstream>

#include <stdexcept>
#include <vector>

int32_t NetworkReader::ReadInt32() const {
    int32_t value = 0;
    _tcpSocket->recvData(&value, sizeof(value));
    return ntohl(value);
}

bool NetworkReader::ReadBool() const {
    bool value = 0;
    _tcpSocket->recvData(&value, sizeof(value));
    return value;
}

std::string NetworkReader::ReadString() const {
    int32_t strSize = this->ReadInt32();

    const unsigned int MAX_BUF_LENGTH = strSize > 4096 ? 4096 : strSize;
    std::vector<char> buffer(MAX_BUF_LENGTH);
    std::string str = "";

    int remainBytes = strSize;
    int recvBytes = 0;
    
    while ((remainBytes > 0) && ((recvBytes = _tcpSocket->recvData(&buffer[0], buffer.size())) > 0)) {
        str.append(buffer.cbegin(), buffer.cend());
        remainBytes -= recvBytes;
    }

    return str;
}

std::vector<std::string> NetworkReader::ReadStringList() const {
    int32_t listSize = this->ReadInt32();
    std::vector<std::string> list{};

    for (int i = 0; i < listSize; i++) {
        list.push_back(this->ReadString());
    }

    return list;
}

void NetworkReader::ReadFile(const std::string& defaultPath) const {
    int32_t fileSize = this->ReadInt32();
    std::string fileName = this->ReadString();
    std::string path = defaultPath + "/" + fileName;

    std::ofstream newFile(path, std::ios::binary);
    if (!newFile.is_open()) {
        throw std::runtime_error("can not open file for write");
    }

    int remain_data = fileSize;
    int received_bytes = 0;
    char buf[BUFSIZ];

    while ((remain_data > 0) && ((received_bytes = _tcpSocket->recvData(buf, BUFSIZ)) > 0)) {
        newFile.write(buf, received_bytes);
        remain_data -= received_bytes;
    }

    newFile.close();
}

NetworkReader::NetworkReader(TCPSocket* tcpSocket) {
    _tcpSocket = tcpSocket;
}