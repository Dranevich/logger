/*
This header file contains realizations of data structures
and functions related to socket statistics processing
*/

#include "log_statistics.hpp"

#include <iostream>
#include <regex>
#include <ctime>

using Clock = std::chrono::system_clock; //is used for tracking time intervals

void LogStats::add(const LogEntry& entry) {
    total_count++; //increments total number of messages received
    level_count[entry.level]++; //increments total number of same level messages received
    total_length += entry.length; //increments total data volume

    //update MAX and MIN values, if needed
    if (entry.length > max_length) max_length = entry.length;
    if (entry.length < min_length || min_length == 0) min_length = entry.length;

    timestamps.push_back(entry.timestamp);
    changed = true; //something happened

    cleanupOldTimestamps();
}

bool LogStats::hasChanged() const {
    return changed;
}

void LogStats::resetChanged() {
    changed = false;
}

void LogStats::cleanupOldTimestamps() {
    auto now = Clock::now();
    while (!timestamps.empty() &&
           std::chrono::duration_cast<std::chrono::minutes>(now - timestamps.front()).count() >= 60) {
        timestamps.pop_front();
    }
}

void LogStats::print() { //prints all required statistics
    cleanupOldTimestamps();

    std::cout << "\t----- CURRENT STATISTICS -----" << std::endl;
    std::cout << "Messages total: " << total_count << std::endl;
    
    std::cout << "Messages by severity level: " << total_count << std::endl;
    for (const auto& [level, count] : level_count) {
        std::cout << " > " << level << ": " << count << std::endl;
    }

    std::cout << "Last hour: " << timestamps.size() << std::endl;

    if (total_count > 0) {
        std::cout << "Message length:";
        std::cout << "  Average: " << (float)(total_length / total_count) << std::endl;
        std::cout << "  MAX: " << max_length << std::endl;
        std::cout << "  MIN: " << min_length << std::endl;
    }

    std::cout << "\t-------------------"<<std::endl;
}

//Parses raw message to separate fields and checks its metadata
LogEntry parseLog(const std::string& line) { 
    std::regex log_regex(R"(\[(.*?)\]\s+\[(.*?)\]\s+(.*))"); //common pattern: 
    std::smatch match;

    if (std::regex_match(line, match, log_regex)) {
        std::string timestamp_str = match[1];
        std::string level = match[2];
        std::string message = match[3];

        std::tm tm = {}; //empty datetime struct
        strptime(timestamp_str.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
        auto tp = Clock::from_time_t(std::mktime(&tm));

        return LogEntry{
            .level = level,
            .timestamp = tp,
            .length = message.length()
        };
    } else { //message might be corrupted or something
        return LogEntry{
            .level = "UNKNOWN",
            .timestamp = Clock::now(),
            .length = line.length()
        };
    }
}




