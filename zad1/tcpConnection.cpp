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

void ConnectionBuffer::send(uint8_t byte) {
    buffer[buffer_pointer++] = byte;
    if (buffer_pointer == buffer_size) {
        flush();
    }
}

void ConnectionBuffer::send(const std::vector<uint8_t>& bytes) {
    unsigned long how_many = bytes.size();
    auto where = buffer + buffer_pointer;
    auto begin = bytes.begin();
    auto end = bytes.begin();
    while(how_many > buffer_size - buffer_pointer) {
        end += buffer_size - buffer_pointer;
        std::copy(begin, end, where);
        begin = end;
        where = buffer + buffer_pointer;
        how_many -= buffer_size - buffer_pointer;
        buffer_pointer = buffer_size;
        flush();
    }
    where = buffer + buffer_pointer;
    std::copy(begin, bytes.end(), where);
    buffer_pointer += std::distance(begin, bytes.end());

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
    auto count = write(socket_val, buffer, buffer_pointer);

    for(int i=0; i<buffer_pointer; ++i) {
        std::cerr<<unsigned(buffer[i])<<" ";
    }
    std::cerr<<std::endl;

    if (count != buffer_pointer) {
        throw TcpException("Error while writing.");
    }
    buffer_pointer = 0;
}

ConnectionBuffer::~ConnectionBuffer() {
    delete[] buffer;
    close(socket_val);
}
