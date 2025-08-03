# Project Title
Logger - a simple file & socket logging toolset

## Description
The goal of this project is to develop a logging library as well as a set of console applications demonstrating its capabilities in accordance with the requirements of the Test Task. The project includes:

1. **Logger** - a library implementing the functions of writing to a log (text file) and sending them via a socket;
2. **Logger_example** - a console application that allows you to write user messages to a text file and socket, as well as filter them by importance;
3. **Log_stat_server** - a console application collecting statistics on data from the socket.

## Features
- Collect user messages marked by chosen severity level and write it to log file
- Collect user messages marked by chosen severity level and send it via socket
- Filter messages by their severity level
- Collect and print socket data statistics, including:
  1. Total number of messages
  2. Number of messages with same severity level
  3. Number of messages recieved last hour
  4. Message length statistics (average, minimum, maximum)

## Requirements

- Debian | Ubuntu OS
- GCC compiler: version >= 9.0
- CMake: version >= 3.10
- C++17 support


## Installation
git clone <репозиторий>
cd LoggerLib
mkdir -p build
cd build

**1. Shared Libraries building**
cmake -DBUILD_SHARED_LIBS=ON ..
make

**2. Static Libraries building**
cmake -DBUILD_SHARED_LIBS=ON ..
make

## Usage
**1. Logger_example**
i.  Initialization
cd logger/build
./logger_example <log_file> <default_severity_level>

Available severity level options: LOW | MEDIUM | HIGH
Please note that Logger_example will igrore all incoming messages marked with lower than default severity level. 
For example, if default severity level is MEDIUM, application would log MEDIUM and HIGH-marked messages, but not LOW.

| Message  |   Default severity level  |
| severity |---------------------------|
|  level   |  LOW  |  MEDIUM  |  HIGH  |
|----------|---------------------------|
|   LOW    |   +   |    +     |   +    |
|  MEDIUM  |   -   |    +     |   +    |
|   HIGH   |   -   |    -     |   +    |

ii. Writing messages
Next, you'll be asked to enter your message as a plain string and chose it's severity level.
Example:

Enter your message:
<message_string>
Enter severity level (LOW|MEDIUM|HIGH)
<chosen_severity_level>

If chosen severity level is invalid or missing, it would be automatically set to default.

**2.Log_stat_server**
i.  Initialization

Open another Terminal instance and execute following comands. Do not close previous console there Logger_example is running!

cd logger/lib
./log_stat_server --ip <ip> --port <port> --count <N> --timeout <T>

|  Parameter |                         Description                      | Default value |
|------------|----------------------------------------------------------|---------------|
|    --ip    |                     Target IPv4 address                  |   0.0.0.0     |
|   --port   |                         Target port                      |     9000      |
|  --count   |            Print stats after N messages received         |       5       |
| --timeout  | Print stats after T seconds if any new data was recieved |      300      |

If any parameters is invalid or missing, they would be automatically set to default values.

ii. Tracking the statistics

Next, you'll have to wait until some message would be recieved. You can send them via Logger_example window.

Statistics would be printed:
a. After recieving every N messages;
b. After every timeout interval if there were any new data.

# About the author
Author: Dranevich Anastasya
Developed as part of the internship Test Task.
Technologies: C++17, Linux, POSIX sockets, STL, multithreading.
