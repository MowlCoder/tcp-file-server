#pragma once

#include "socket.h"

#include <string>
#include <cstdlib>
#include <memory>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
#endif

class TCPSocket {
private:
    std::shared_ptr<Socket> _socket;
    addrinfo* _addr;
    const std::string _ip;
    const std::string _port;
public:
    void initSocket();
    void closeSocket();

    const int getSocketFd() const;
    std::shared_ptr<Socket> getSocket() const;
    const addrinfo* getAddr() const;

    TCPSocket(const std::string& ip, const std::string& port) : _ip(ip), _port(port) {}
};