//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#ifndef ZAD1_TCPCONNECTION_H
#define ZAD1_TCPCONNECTION_H

#include <netinet/in.h>
#include <stdexcept>

const int QUEUE_LENGTH = 10;
const long BUFFER_LENGTH = 2048;

class TcpException : public std::runtime_error {

public:
    explicit TcpException(const std::string& msg) : std::runtime_error(msg) {}

};

class Eof: public std::exception {};

class ConnectionBuffer {
    int socket_val;
    struct sockaddr_in address;
    unsigned buffer_size;
    uint8_t *buffer;
    unsigned buffer_pointer;

public:
    ConnectionBuffer(int socket_val, struct sockaddr_in address, unsigned buffer_size = BUFFER_LENGTH)
            : socket_val(socket_val), buffer_size(buffer_size), address(address), buffer_pointer(0),
              buffer(new uint8_t[buffer_size]) {}

    std::string toString() const;

    void send(uint8_t byte);

    void send(const std::vector<uint8_t>& bytes);

    uint8_t recieve();

    std::vector<uint8_t> recieve(unsigned limit);

    void flush();

    ~ConnectionBuffer();

};


class tcpConnection {
    int socket_val;
    struct sockaddr_in address;
public:
    explicit tcpConnection(long port, int queue_length = QUEUE_LENGTH);

    ConnectionBuffer handleClinet();

    ~tcpConnection() noexcept;

};


#endif //ZAD1_TCPCONNECTION_H
