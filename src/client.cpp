#include "network/tcpClient.h"
#include "network/errors.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const std::string PORT = "7654";

int main(void) {
    TCPClient tcpClient = TCPClient(PORT);

    if (!tcpClient.connectToServer()) {
        return 1;
    }

    std::cout << "Commands: list, upload, download, quit" << std::endl;

    while (true) {
        try {
            std::string command;
            std::cout << "> ";
            std::cin >> command;

            if (command == "quit") {
                break;
            } else if (command == "list") {
                tcpClient.writer()->WriteInt32(1);
                std::vector<std::string> fileNames = tcpClient.reader()->ReadStringList();

                std::cout << "Files on server:" << std::endl;

                for (const auto& name : fileNames) {
                    std::cout << "- " << name << std::endl;
                }
            } else if (command == "upload") {
                std::string filePath, fileName;
                std::cout << "Enter path to file for uploading: ";
                std::cin >> filePath;
                std::cout << "Enter name for file on the server: ";
                std::cin >> fileName;

                if (!std::filesystem::exists(filePath)) {
                    std::cout << "ERROR: file " << filePath << " not found" << std::endl;
                } else {
                    tcpClient.writer()->WriteInt32(2);
                    tcpClient.writer()->WriteFile(filePath, fileName);
                }
            } else if (command == "download") {
                std::string fileName, filePath;
                std::cout << "Enter file name to download: ";
                std::cin >> fileName;
                std::cout << "Enter path to directory, where download a file: ";
                std::cin >> filePath;

                tcpClient.writer()->WriteInt32(3);
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