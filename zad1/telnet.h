//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#ifndef ZAD1_TELNET_H
#define ZAD1_TELNET_H


#include <cstdint>
#include <vector>
#include "tcpConnection.h"

enum Keys {
    PRINT,
    UP,
    DOWN,
    ENTER,
    STREAM_END,
    DUMMY
};

struct Command {

    static const uint8_t CMD_WILL = 251;
    static const uint8_t CMD_WONT = 252;
    static const uint8_t CMD_DO = 253;
    static const uint8_t CMD_DONT = 254;
    static const uint8_t CMD_NOP = 241;
    static const uint8_t CMD_SE = 240;
    static const uint8_t CMD_SB = 250;
    static const uint8_t CMD_IAC = 255;
    static const uint8_t CMD_UNKNOWN = 0;


    static const uint8_t OPT_ECHO = 1;
    static const uint8_t OPT_LINEMODE = 34;
    static const uint8_t OPT_UNKNOWN = 254;

};

class Telnet {

    ConnectionBuffer connectionBuffer;
public:
    Telnet(const ConnectionBuffer& connectionBuffer);

public:
    std::vector<uint8_t> init();


    std::vector<uint8_t> response(const std::vector<uint8_t>& telnet_command);




    Keys readAsciEscape();

    Keys readCommand();

    Keys processCsiSequence();
};


#endif //ZAD1_TELNET_H
