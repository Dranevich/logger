cmake_minimum_required(VERSION 3.10) 
project(LoggerLib VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared library" ON)

include_directories(include)

add_library(loggerlib
    src/logger.cpp 
    src/socket_writer.cpp 
)

target_include_directories(loggerlib PUBLIC ${PROJECT_SOURCE_DIR}/include)

install(TARGETS loggerlib DESTINATION lib)
install(FILES include/logger.hpp DESTINATION include)
install(FILES tools/log_statistics.hpp DESTINATION include)

add_executable(logger_example examples/main.cpp)
add_executable(log_stat_server tools/log_stat_server.cpp tools/log_statistics.cpp)



target_link_libraries(logger_example loggerlib)
target_include_directories(logger_example PRIVATE ${PROJECT_SOURCE_DIR}/include)


