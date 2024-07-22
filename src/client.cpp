#include "network/tcpClient.h"
#include "network/errors.h"

#include "commands/commandType.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const std::string IP = "127.0.0.1";
const std::string PORT = "7654";

int main(void) {
    TCPClient tcpClient = TCPClient(IP, PORT);

    if (!tcpClient.connectToServer()) {
        return 1;
    }

    std::cout << "Commands: list, upload, download, quit" << std::endl;

    while (true) {
        try {
            std::string commandStr;
            std::cout << "> ";
            std::cin >> commandStr;
            Command cmd = commandFromString(commandStr);

            switch (cmd)
            {
            case Command::QUIT:
                return 0;
            case Command::LIST: {
                tcpClient.writer()->WriteInt32(commandToInt32(cmd));
                std::vector<std::string> fileNames = tcpClient.reader()->ReadStringList();

                std::cout << "Files on server:" << std::endl;

                for (const auto& name : fileNames) {
                    std::cout << "- " << name << std::endl;
                }

                break;
            }
            case Command::UPLOAD: {
                std::string filePath, fileName;
                std::cout << "Enter path to file for uploading: ";
                std::cin >> filePath;
                std::cout << "Enter name for file on the server: ";
                std::cin >> fileName;

                if (!std::filesystem::exists(filePath)) {
                    std::cout << "ERROR: file " << filePath << " not found" << std::endl;
                } else {
                    tcpClient.writer()->WriteInt32(commandToInt32(cmd));
                    tcpClient.writer()->WriteFile(filePath, fileName);
                }

                break;
            }
            case Command::DOWNLOAD: {
                std::string fileName, filePath;
                std::cout << "Enter file name to download: ";
                std::cin >> fileName;
                std::cout << "Enter path to directory, where download a file: ";
                std::cin >> filePath;

                tcpClient.writer()->WriteInt32(commandToInt32(cmd));
                tcpClient.writer()->WriteString(fileName);

                bool status = tcpClient.reader()->ReadBool();

                if (status) {
                    if (!std::filesystem::exists(filePath)) {
                        std::filesystem::create_directories(filePath);
                    }

                    tcpClient.reader()->ReadFile(filePath);
                } else {
                    std::cout << "ERROR: file " << fileName << " not exists on server" << std::endl;
                }
            }
            default:
                std::cout << "WARNING: unknown command" << std::endl;
                break;
            }
        } catch (const SocketClosedException& e) {
            std::cout << "ERROR: connection with server was interrupted" << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
            break;
        }
    }

    return 0;
}