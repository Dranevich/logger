/*
This header file contains definitions of data structures
and functions related to socket statistics processing
*/

#pragma once

#include <string>
#include <chrono>
#include <map>
#include <deque>

struct LogEntry { //single message
    //we don't need to store message itself, but it's metadata
    std::string level;
    std::chrono::system_clock::time_point timestamp;
    size_t length;
};

class LogStats { 
public:
    void add(const LogEntry& entry);
    void print();
    bool hasChanged() const;
    void resetChanged();

private:
    void cleanupOldTimestamps();

    size_t total_count = 0;
    size_t total_length = 0;
    size_t max_length = 0;
    size_t min_length = 0;
    std::map<std::string, size_t> level_count;

    // last hour only
    std::deque<std::chrono::system_clock::time_point> timestamps;

    bool changed = false;
};

// Parses raw string to LogEntry object
LogEntry parseLog(const std::string& line);


