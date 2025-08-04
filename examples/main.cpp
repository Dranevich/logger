#include <iostream>
#include <string>
#include <logger.hpp>
#include <socket_writer.hpp>
#include <thread>

using namespace loggerlib;

LogLevel parseLogLevel(const std::string& level_str){ //parses input string to enum value
  std::string lvl = level_str;
  std::transform(lvl.begin(), lvl.end(), lvl.begin(), ::toupper); //ignores upper/lower case
  
  if (lvl == "LOW") return LogLevel::LOW;
  else if (lvl == "MEDIUM") return LogLevel::MEDIUM;
  else if (lvl == "HIGH") return LogLevel::HIGH;
  else throw std::invalid_argument("Invalid input");
}

void getUserData(Logger& logger){ //collects the data from user
  std::string msg;
  std::string lvl_str;
  
  std::cout<<"Enter the message: "<< std::endl;
  std::getline(std::cin, msg);
  
  std::cout<<"Enter severity level (LOW|MEDIUM|HIGH): "<< std::endl;
  std::cout<<"(Default: "<< logger.logLevelToString(logger.getDefaultLevel())<<")"  << std::endl;
  
  std::getline(std::cin, lvl_str);
  std::transform(lvl_str.begin(), lvl_str.end(), lvl_str.begin(), ::toupper); //ignores upper/lower case
  
  if (hasCorrectLvl(lvl_str)){ //if chosen level is LOW, MEDIUM or HIGH, apply user value
    LogLevel lvl = parseLogLevel(lvl_str);
    logger.log(msg, lvl);
  } else {  //else: set default value
    std::cout<<"> Warning: "<< lvl_str << " is incorrect severity parameter. Message severity level has been set to default ("<< logger.logLevelToString(logger.getDefaultLevel())<<")."<<std::endl;
    logger.log(msg);
  }
  
}


int main(int argc, char* argv[]) {

  if (argc!=3) {
    std::cerr << "Usage: "<< argv[0] << "[path-to-the-log-file] [severity-level]"<< std::endl;
    std::cerr << "Example: "<< argv[0] << "log.txt LOW"<< std::endl;
    return 1;
  }
  
  std::string filename = argv[1];
  std::string level_str = argv[2];
  std::transform(level_str.begin(), level_str.end(), level_str.begin(), ::toupper); //ignores upper/lower case
  
   if (hasCorrectLvl(level_str)){
      std::cout<<"Program has been successfully started" << std::endl;
      std::cout<<"Default severity level: "<< level_str << std::endl;
      std::cout<<"Output file name: " << filename << std::endl;
      LogLevel level = parseLogLevel(level_str);
      Logger logger (filename, level);
      
      //enables socket logging
      //comment if needed
      logger.enableSocketLogging("127.0.0.1", 9000); 
      
      while (true) {
        getUserData(logger);
      }
      
    } else {
      std::cout<<"Available options: [LOW|MEDIUM|HIGH]" << std::endl;
      return 1;
    }
    return 0;
}

