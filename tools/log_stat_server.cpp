#include "log_statistics.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>
#include <getopt.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using Clock = std::chrono::system_clock; //is used for tracking time intervals

volatile bool running = true; //this value might be changed externally
void handleSignal(int) { running = false; }

int main(int argc, char* argv[]) {
    //sets default parameters
    std::string ip = "0.0.0.0"; //default value: listen own address
    int port = 9000; // that's UDP
    int N = 5; //prints statistics every 5 messages
    int T = 300; //default timeout (in seconds)

    //allows user to set all or chosen parameters
    const struct option long_opts[] = { 
        {"ip", required_argument, nullptr, 'i'},
        {"port", required_argument, nullptr, 'p'},
        {"count", required_argument, nullptr, 'n'},
        {"timeout", required_argument, nullptr, 't'},
        {nullptr, 0, nullptr, 0}
    };

    int opt; //just an iterator
    while ((opt = getopt_long(argc, argv, "i:p:n:t:", long_opts, nullptr)) != -1) {
        switch (opt) {
            case 'i': ip = optarg; break;
            case 'p': port = std::stoi(optarg); break;
            case 'n': N = std::stoi(optarg); break;
            case 't': T = std::stoi(optarg); break;
            default:
                std::cerr << "Usage: " << argv[0]
                          << " --ip <IPv4-address> --port <port-number> --count <frequency> --timeout <value-in-seconds>"<<std::endl;
                return 1;
        }
    }

    signal(SIGINT, handleSignal); //stops the process

    //another socket instance to listen incoming messages
    //also uses Internet address domain and processes datagrams with default protocol
    int sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) {
        std::cerr << "Not able to create new socket";
        return 1;
    }

    sockaddr_in server_addr{}; //stores target IP
    server_addr.sin_family = AF_INET; // that's IPv4
    server_addr.sin_port = htons(port); // selected port
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); //converts human-readable IP to it's network standard

    //binds socket with target IP
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Not able to bind Socket with " << ip << ":" << port << std::endl;
        return 1;
    }
    
    std::cout << "Server had been successfully initialized!" << std::endl;
    std::cout << "Target endpoint: " << ip << " : " << port << std::endl;

    LogStats stats;
    int messages_since_last_print = 0;
    auto last_print_time = Clock::now();

    while (running) {
        fd_set read_fds; //file descriptor
        FD_ZERO(&read_fds); //clear the memory
        FD_SET(sock, &read_fds); //adds socket to the set

        timeval timeout{};
        timeout.tv_sec = 1; //timestamp accurancy
        timeout.tv_usec = 0;

        //checks current socket state
        int activity = select(sock + 1, &read_fds, nullptr, nullptr, &timeout);
        if (activity > 0 && FD_ISSET(sock, &read_fds)) { //socket is OK
            char buffer[2048] = {};
            ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0); //gets incoming socket data
            if (bytes > 0) {
                std::string msg(buffer, bytes); 
                //prints received payload, but doesn't store it
                std::cout << msg << std::endl;

                //collect metadata
                LogEntry entry = parseLog(msg);
                stats.add(entry);
                messages_since_last_print++;

                //if required number of messages is already recieved 
                if (messages_since_last_print >= N) {
                    stats.print(); //prints statistic
                    stats.resetChanged(); //set flag to zero
                    messages_since_last_print = 0; //set counter to zero
                    last_print_time = Clock::now(); //update the timer
                }
            }
        }

        auto now = Clock::now(); //checks current system time
        //checks if there were any new messages and time interval is over
        if (stats.hasChanged() &&
            std::chrono::duration_cast<std::chrono::seconds>(now - last_print_time).count() >= T) { 
            stats.print();
            stats.resetChanged();
            messages_since_last_print = 0;
            last_print_time = now;
        }
    }

    std::cout << "----- EXIT -----"<<std::endl;
    close(sock);
    return 0;
}




