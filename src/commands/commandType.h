#pragma once

#include <string>

enum class Command {
    UNKNOWN,
    LIST,
    UPLOAD,
    DOWNLOAD,
    QUIT
};

Command commandFromString(const std::string& str) {
    if (str == "list") {
        return Command::LIST;
    }

    if (str == "upload") {
        return Command::UPLOAD;
    }

    if (str == "download") {
        return Command::DOWNLOAD;
    }

    if (str == "quit") {
        return Command::QUIT;
    }

    return Command::UNKNOWN;
}

int32_t commandToInt32(Command cmd) {
    return static_cast<int32_t>(cmd);
}

Command int32ToCommand(int32_t val) {
    return static_cast<Command>(val);
}