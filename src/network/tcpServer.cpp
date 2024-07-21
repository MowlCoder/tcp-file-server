#include "tcpServer.h"

#include <iostream>
#include <signal.h>

void TCPServer::acceptConnections(ClientHandler handler) {
    while (true) {
        sockaddr_storage connected_addr;
        socklen_t connected_addr_size = sizeof(connected_addr);
        int connected_fd = accept(_socket->getSocketFd(), (sockaddr*)&connected_addr, &connected_addr_size);

        if (connected_fd < 0) {
            continue;
        }

        int* clientFdArg = new int(connected_fd);

        pthread_t thread_id;
        pthread_create(&thread_id, nullptr, handler, clientFdArg);
        pthread_detach(thread_id);
    }
}

bool TCPServer::serve() {
    signal(SIGPIPE, SIG_IGN);

    int socket_reuse = 1;
    if (setsockopt(_socket->getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &socket_reuse, sizeof(int)) == -1) {
        std::cout << "ERROR can't set socket option to reuse port" << std::endl;
        return false;
    }

    if (bind(_socket->getSocketFd(), _addr->ai_addr, _addr->ai_addrlen) < 0) {
        std::cout << "ERROR: can not bind socket to port" << std::endl;
        return false;
    }

    if (listen(_socket->getSocketFd(), BACKLOG) < 0) {
        std::cout << "ERROR: can not listen" << std::endl;
        return false;
    }

    return true;
}