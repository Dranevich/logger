/*
This header file contains definitions for essential
data structures and functions of logging library
*/
#pragma once

//all libraries listed below are parts of STL 
#include <string>
#include <fstream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>
#include <ctime>
#include <memory>
#include <algorithm>

namespace loggerlib {

class SocketWriter;

//Severity levels
enum class LogLevel {
    LOW,
    MEDIUM,
    HIGH
};

class Logger { //logging agent
public:
    Logger(const std::string& filename, LogLevel default_level);
    ~Logger();

    void enableSocketLogging(const std::string& ip, uint16_t port);

    void log(const std::string& message, LogLevel level);
    void log(const std::string& message);
    void setDefaultLevel(LogLevel level); //changes default logging level if needed
    LogLevel getDefaultLevel() {return default_level_;}; //returns default log level value 

    static std::string logLevelToString(LogLevel level);
    static std::string getCurrentTimeString();

private:
    void processQueue(); //is used for multithreading

    std::ofstream logfile_;
    LogLevel default_level_;

    std::mutex queue_mutex_; //marks resource as busy
    std::condition_variable cv_; //checks if output file is free or busy
    std::queue<std::string> message_queue_; //buffer
    std::thread worker_; //current thread
    std::atomic<bool> running_; //prevents CPU from operation reordering

    std::unique_ptr<SocketWriter> socket_writer_; //socket logging module, if needed
};


bool hasCorrectLvl (const std::string& level_str); //checks user values

} 



