#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

#include "network/writer.h"
#include "network/reader.h"
#include "network/errors.h"
#include "network/tcpServer.h"

const std::filesystem::path FILE_DIR_PATH = std::filesystem::path("./static");
const std::string PORT = "7654";

void* handleClient(void* arg) {
    int clientSocketFd = *((int*)arg);

    TCPSocket* tcpSocket = new TCPSocket(clientSocketFd);
    NetworkReader netReader = NetworkReader(tcpSocket);
    NetworkWriter netWriter = NetworkWriter(tcpSocket);

    std::cout << "INFO: new connection with socket " << clientSocketFd << std::endl;

    while (true) {
        try {
            int32_t commandType = netReader.ReadInt32();

            if (commandType == 0) {
                break;
            }

            if (commandType == 1) {
                std::vector<std::string> fileNames{};

                for (const auto& entry : std::filesystem::directory_iterator(FILE_DIR_PATH)) {
                    if (entry.is_regular_file()) {
                        fileNames.push_back(entry.path().filename());
                    }
                }

                netWriter.WriteStringList(fileNames);
            } else if (commandType == 2) {
                netReader.ReadFile(FILE_DIR_PATH.string());
            } else if (commandType == 3) {
                auto filename = std::filesystem::path(netReader.ReadString());

                if (!std::filesystem::exists(FILE_DIR_PATH / filename)) {
                    netWriter.WriteBool(false);
                } else {
                    netWriter.WriteBool(true);
                    netWriter.WriteFile(FILE_DIR_PATH / filename, filename);
                }
            }
        } catch (const SocketClosedException& e) {
            break;
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
            break;
        }
    }

    std::cout << "INFO: connection was closed with socket " << clientSocketFd << std::endl;

    close(clientSocketFd);
    delete tcpSocket;
    delete (int*)arg;

    return nullptr;
}

int main(void) {
    TCPServer server = TCPServer(PORT);

    if (!server.serve()) {
        return 1;
    }

    std::cout << "INFO: server is listening on port " << PORT << std::endl;

    server.acceptConnections(handleClient);

    return 0;
}