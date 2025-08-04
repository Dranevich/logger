/*
This header file contains realizations of data structures
and functions related to socket logging module
*/
#include "socket_writer.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <iostream>

namespace loggerlib {

SocketWriter::SocketWriter(const std::string& ip, uint16_t port) {
    //system call for a socket creation
    //it uses Internet address domain, send datagrams and uses default protocol
    sock_ = socket(AF_INET, SOCK_DGRAM, 0); 
    
    std::cout<<"INFO: Socket was successfully created" << std::endl;
    
    if (sock_ < 0) {
        throw std::runtime_error("Error: not able to create new socket to send data!");
    }

    std::memset(&server_addr_, 0, sizeof(server_addr_)); //copies IP address
    server_addr_.sin_family = AF_INET; //marks address as IPv4
    server_addr_.sin_port = htons(port); //converts port number value to Network Byte Order

    if (inet_pton(AF_INET, ip.c_str(), &server_addr_.sin_addr) <= 0) {
        throw std::invalid_argument("Invalid IP: " + ip); //
    }
}

SocketWriter::~SocketWriter() {
    close(sock_); //closes socket before exiting
}

void SocketWriter::send(const std::string& message) {
    //also uses sendto() system call
    sendto(sock_, message.c_str(), message.size(), 0,
           reinterpret_cast<sockaddr*>(&server_addr_), sizeof(server_addr_));
}

} // namespace loggerlib


