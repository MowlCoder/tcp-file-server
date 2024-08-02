#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>

#include "network/writer.h"
#include "network/reader.h"
#include "network/errors.h"
#include "network/tcpServer.h"

#include "commands/commandType.h"

const std::filesystem::path FILE_DIR_PATH = std::filesystem::path("./static");
const std::string PORT = "7654";

void* handleClient(void* arg) {
    int clientSocketFd = *((int*)arg);

    auto socket = std::make_shared<Socket>(clientSocketFd);
    NetworkReader netReader = NetworkReader(socket);
    NetworkWriter netWriter = NetworkWriter(socket);

    std::cout << "INFO: new connection with socket " << clientSocketFd << std::endl;

    while (true) {
        try {
            int32_t commandType = netReader.ReadInt32();
            Command cmd = int32ToCommand(commandType);

            if (cmd == Command::UNKNOWN) {
                break;
            }

            switch (cmd)
            {
            case Command::LIST: {
                std::vector<std::string> fileNames{};

                for (const auto& entry : std::filesystem::directory_iterator(FILE_DIR_PATH)) {
                    if (entry.is_regular_file()) {
                        fileNames.push_back(entry.path().filename().string());
                    }
                }

                netWriter.WriteStringList(fileNames);

                break;
            }
            case Command::UPLOAD: {
                netReader.ReadFile(FILE_DIR_PATH.string());
                break;
            }
            case Command::DOWNLOAD: {
                auto filename = std::filesystem::path(netReader.ReadString());

                if (!std::filesystem::exists(FILE_DIR_PATH / filename)) {
                    netWriter.WriteBool(false);
                } else {
                    netWriter.WriteBool(true);
                    netWriter.WriteFile((FILE_DIR_PATH / filename).string(), filename.string());
                }

                break;
            }
            default:
                std::cout << "ERROR: receive invalid command " << commandType << std::endl;
                break;
            }
        } catch (const SocketClosedException& e) {
            break;
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
            break;
        }
    }

    std::cout << "INFO: connection was closed with socket " << clientSocketFd << std::endl;

    socket->close();
    delete (int*)arg;

    return nullptr;
}

int main(void) {
    TCPServer server = TCPServer(PORT);
    if (!server.serve()) {
        return 1;
    }

    if (!std::filesystem::exists(FILE_DIR_PATH)) {
        std::filesystem::create_directories(FILE_DIR_PATH);
    }

    std::cout << "INFO: server is listening on port " << PORT << std::endl;

    server.acceptConnections(handleClient);

    return 0;
}