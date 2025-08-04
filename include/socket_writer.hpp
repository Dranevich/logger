/*
This header file contains definitions of data structures
and functions related to socket logging module
*/

#pragma once

#include <string>
#include <netinet/in.h>

namespace loggerlib {

class SocketWriter {
public:
    SocketWriter(const std::string& ip, uint16_t port);
    ~SocketWriter();

    void send(const std::string& message);

private:
    int sock_;
    sockaddr_in server_addr_;
};

} // namespace loggerlib

