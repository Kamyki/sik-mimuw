#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "tcpConnection.h"
#include <iostream>

tcpConnection::tcpConnection(long port, int queue_length) {
    this->socket_val = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_val < 0) {
        throw TcpException("Error when invoking socket");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    int yes=1;
    if (setsockopt(socket_val, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        throw TcpException("Error when  invoking socketopt");

    if (bind(socket_val, (struct sockaddr *) &address, sizeof(address)) < 0) {
        throw TcpException("Error when invoking bind");
    }

    if (listen(socket_val, queue_length) < 0) {
        throw TcpException("Error when invoking listen");
    }
}

tcpConnection::~tcpConnection() noexcept {
    close(socket_val);
}

ConnectionBuffer tcpConnection::handleClinet() {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(socket_val, (struct sockaddr *) &client_address, &client_address_len);
    if (client_socket < 0) {
        throw TcpException("Error when accpeting client connection");
    }

    return {client_socket, client_address};
}


std::string ConnectionBuffer::toString() const {
    return std::string(inet_ntoa(address.sin_addr));
}

void ConnectionBuffer::write(uint8_t byte) {
    buffer[buffer_pointer++] = byte;
    if (buffer_pointer == buffer_size) {
        flush();
    }
}

void ConnectionBuffer::write(const std::vector<uint8_t>& bytes) {
    for(auto byte: bytes) {
        write(byte);
    }
    flush();
}

void ConnectionBuffer::write(const std::string& str) {
    for(auto byte: str) {
        write(static_cast<uint8_t>(byte));
    }
    flush();
}

uint8_t ConnectionBuffer::recieve() {
    auto bytes = this->recieve(1);
    return bytes[0];
}

std::vector<uint8_t> ConnectionBuffer::recieve(unsigned limit) {
    std::vector<uint8_t> bytes(limit);
    auto read_count = read(socket_val, bytes.data(), limit);
    if (read_count < 0) {
        throw TcpException("Error while reading");
    } else if (read_count == 0) {
        throw Eof();
    }
    bytes.resize((unsigned long) read_count);
    return bytes;
}

void ConnectionBuffer::flush() {
    auto count = send(socket_val, buffer, buffer_pointer, 0);

    if (count != buffer_pointer) {
        throw TcpException("Error while writing.");
    }
    buffer_pointer = 0;
}

ConnectionBuffer::~ConnectionBuffer() {
    delete[] buffer;
    close(socket_val);
}
