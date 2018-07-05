//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#include "telnet.h"
#include "state.h"

std::vector<uint8_t> Telnet::init() {

    return {
            Command::CMD_IAC,
            Command::CMD_WILL,
            Command::OPT_ECHO,
            Command::CMD_IAC,
            Command::CMD_DO,
            Command::OPT_LINEMODE
    };
}

std::vector<uint8_t> Telnet::response(const std::vector<uint8_t>& telnet_command) {
    auto type = telnet_command[1];
    auto arg = telnet_command[2];
    std::vector<uint8_t> seq {
            Command::CMD_IAC,
            Command::CMD_WONT,
            arg
    };
    if (type == Command::CMD_DONT && arg != Command::OPT_ECHO) {
        return seq;
    } else if (type == Command::CMD_DO && arg != Command::OPT_ECHO) {
        return seq;
    } else if (type == Command::CMD_WONT && arg != Command::OPT_LINEMODE) {
        return seq;
    } else if (type == Command::CMD_WILL && arg != Command::OPT_LINEMODE) {
        return seq;
    }
    return {};

}

Telnet::Telnet(const ConnectionBuffer& connectionBuffer) : connectionBuffer(connectionBuffer) {}

bool isPrintable(uint8_t byte) {
    return byte >= 32 && byte <= 126;
}

Keys Telnet::readCommand() {
    uint8_t byte;

    try {
        byte = connectionBuffer.recieve();
    } catch (const Eof& eof_exception) {
        return Keys::STREAM_END;
    }

    if (isPrintable(byte)) {
        return Keys::PRINT;
    } else if (byte == ESCAPE) {
        auto tmp = readAsciEscape();
        if(tmp == Keys::DUMMY) {
            return readCommand();
        } else {
            return tmp;
        }
    } else if (byte == CR) {
        return Keys ::ENTER;
    } else {
        return readCommand();
    }
}

Keys Telnet::readAsciEscape() {
    uint8_t byte;
    try {
        byte = connectionBuffer.recieve();
    } catch (const Eof& eof_exception) {
        return Keys::STREAM_END;
    }
    if (byte == '[') {
        return processCsiSequence();
    } else {
        return readCommand();
    }
}

Keys Telnet::processCsiSequence() {
    uint8_t byte;
    try {
        byte = connectionBuffer.recieve();
    } catch (const Eof &eof_exception) {
        return Keys::STREAM_END;
    }
    switch (byte) {
        case 'A': {
            return Keys ::UP;
        }
        case 'B': {
            return Keys ::DOWN;
        }
        default: {
            return Keys::DUMMY;
        }
    }
}

