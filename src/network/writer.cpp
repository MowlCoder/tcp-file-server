#include "writer.h"
#include "errors.h"

#include <arpa/inet.h>

#include <stdexcept>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <iostream>

void NetworkWriter::WriteInt32(int32_t val) const {
    int32_t networkVal = htonl(val);
    _tcpSocket->sendData(&networkVal, sizeof(networkVal));
}

void NetworkWriter::WriteBool(bool val) const {
    _tcpSocket->sendData(&val, sizeof(val));
}

void NetworkWriter::WriteStringList(const std::vector<std::string>& list) const {
    this->WriteInt32(list.size());

    for (const auto& entry : list) {
        this->WriteString(entry);
    }
}

void NetworkWriter::WriteString(const std::string& val) const {
    this->WriteInt32(val.size());

    int remainBytes = val.size();
    int offset = 0;
    int sendBytes = 0;
    int bytesToSend = val.size() > 4096 ? 4096 : val.size();

    while ((remainBytes > 0) && ((sendBytes = _tcpSocket->sendData((void*)(val.c_str() + offset), bytesToSend)) > 0)) {
        remainBytes -= sendBytes;
        offset += sendBytes;
    }
}

void NetworkWriter::WriteFile(const std::string& path, const std::string& filename) const {
    struct stat fileStat;

    if (stat(path.c_str(), &fileStat) < 0) {
        throw std::runtime_error("can not get file stat");
    }

    this->WriteInt32(fileStat.st_size);
    this->WriteString(filename);

    std::ifstream inputFile(path);

    int remainData = fileStat.st_size;
    int sendBytes = 0;
    int bufSize = fileStat.st_size > 4096 ? 4096 : fileStat.st_size;
    char buf[bufSize];

    while (remainData > 0) {
        inputFile.read(buf, bufSize);
        sendBytes = _tcpSocket->sendData(buf, bufSize);

        if (sendBytes > 0) {
            remainData -= sendBytes;
        }
    }
}

NetworkWriter::NetworkWriter(TCPSocket* tcpSocket) {
    _tcpSocket = tcpSocket;
}