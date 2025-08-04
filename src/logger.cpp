/*
This header file contains realizations of essential
logging library's functions & methods
*/

#include <iostream>
#include <chrono>
#include <ctime>
#include "socket_writer.hpp"
#include "logger.hpp"


namespace loggerlib {

//Constructor
Logger::Logger(const std::string& filename, LogLevel default_level)
    : default_level_(default_level), running_(true) {
    logfile_.open(filename, std::ios::app);
    worker_=std::thread(&Logger::processQueue, this);
}

//Destructor
Logger::~Logger() {
    {
        //marks thread as finished
        std::lock_guard<std::mutex> lock(queue_mutex_);
        running_ = false;
    }
    cv_.notify_all(); //returns control to next waiting thread, if exist
    if (worker_.joinable()) {
        worker_.join();
    }
    if (logfile_.is_open()) { //closes output stream
        logfile_.close();
    }
}


//allows to add new message to the log file with chosen severity level
void Logger::log(const std::string& message, LogLevel level) {
    if (level < default_level_) {
      return; //skips messages with lower severity
    }

    std::string full_msg = "[" + getCurrentTimeString() + "] [" +
                           logLevelToString(level) + "] " + message;

    { //locks the mutex
        std::lock_guard<std::mutex> lock(queue_mutex_);
        message_queue_.push(full_msg); //keeps messages until they'll be written to the file
    }

    cv_.notify_one(); //now resource is free for next waiting thread's usage
}

//allows to add new message to the log file with default severity level
void Logger::log(const std::string& message) {
    log(message, default_level_);
}


//Default severity level setters
void Logger::setDefaultLevel(LogLevel level) {
    default_level_ = level;
}

void Logger::processQueue() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex_); //marks log file for own private usage
        cv_.wait(lock, [this]() { //waits until log file is free
            return !message_queue_.empty() || !running_;
        });

        while (!message_queue_.empty()) { //now Logger is able to write received messages to the file
            std::string msg = std::move(message_queue_.front());
            message_queue_.pop();
            lock.unlock();

            if (logfile_.is_open()) {
                logfile_ << msg << std::endl; //file output
            }
            
            if (socket_writer_) { //if socket logging is enabled, writes both
              socket_writer_->send(msg);
            }

            lock.lock();
        }

        if (!running_ && message_queue_.empty()) { //no messages left
            break;
        }
    }
}

//converts current system time to a string
std::string Logger::getCurrentTimeString() { 
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return buf;
}

//converts enum values to human-readable string
std::string Logger::logLevelToString(LogLevel num) {
    std::string str;
    if (num==loggerlib::LogLevel::LOW) str="LOW";
    else if (num==loggerlib::LogLevel::MEDIUM) str="MEDIUM";
    else if (num==loggerlib::LogLevel::HIGH) str="HIGH";
    return str;
}

//checks user input
bool hasCorrectLvl (const std::string& lvl){
  if (lvl == "LOW" || lvl == "MEDIUM" || lvl =="HIGH") return true;
  else return false;
}

//allows to enable socket logging if needed
void Logger::enableSocketLogging(const std::string& ip, uint16_t port) {
    socket_writer_ = std::make_unique<SocketWriter>(ip, port);
}


} 

