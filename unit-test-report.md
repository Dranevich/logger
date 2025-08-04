# Unit test report


The purpose of this testing is to control the compliance of individual modules of the developed project with the requirements of the Test Task, as well as to handle exceptional situations.

**Basic Requirements:**
- The developed library should have two build options: static and shared
- The library should provide the ability of journal & socket logging
- The library should provide the ability to filter messages by their severity level
- The program should correctly process any kind of input data
- The program should provide the output of statistical data in accordance with the requirements of the Test Task.

**Test bench configuration:**

- VirtualBox (v.7.1.10) VM
- Ubuntu 24.04.2 LTS
- Intel Core i7-14700HX x 6
- 13.8 GiB Memory

**Test scenario:**

User-story, i.e. simulation of user actions during software installation and execution.

**Result:**

27/27 tests passed

## Test 0: Build types

**Test Description**

Trying to build project with both types (static/dynamic) of libraries.

**Test Requrements**

Project should be successfully built in both cases.

|    Description     |             Input values         | Expected behaviour|  Real behaviour  |
|--------------------|----------------------------------|-------------------|------------------|
|   Static Library   | cmake -DBUILD_SHARED_LIBS=OFF .. |       Success     | Matches expected |
|   Shared Library   | cmake -DBUILD_SHARED_LIBS=ON ..  |       Success     | Matches expected |

**Screenshots**

![Test Zero-shared](https://github.com/Dranevich/logger/blob/main/screens/Test01.jpg)

![Test Zero-static](https://github.com/Dranevich/logger/blob/main/screens/Test02.jpg)

## Test 1: Running the program with different sets of input parameters

**Test Description**

Trying to run logger_example with varius input parameters

**Test Requrements**

If parameters are correct, programm shoulg run succsessfully, overwise return error message.

|            Description         |        Input values        |      Expected behaviour   |  Real behaviour  |
|--------------------------------|----------------------------|---------------------------|------------------|
|          No parameters         |      ./logger_example      |       Error message       | Matches expected |
|          One parameter         |  ./logger_example myfile   |       Error message       | Matches expected |
|        Invalid parameter       |  ./logger_example aaa bbb  |       Error message       | Matches expected |
| Correct parameters, upper case |  ./logger_example aaa LOW  | Successful initialization | Matches expected |
|Correct parameters, various case|./logger_example aaa Medium | Successful initialization | Matches expected |

**Screenshots**

![Test One](https://github.com/Dranevich/logger/blob/main/screens/TestOne.jpg)

## Test 2: Message Filtering
**Test Description**

Trying to write different combinations of default and message severity levels.

**Test Requrements**

Program should log all messages marked same as default severity level or higher, and ignore others. 

**Default Severity Level: LOW**
| Message severity level |   Input values  |                  Expected behaviour               |  Real behaviour  |
|------------------------|-----------------|---------------------------------------------------|------------------|
|           LOW          |       Low       |           Writes message marked as LOW            | Matches expected |
|          MEDIUM        |      medium     |          Writes message marked as MEDIUM          | Matches expected |
|           HIGH         |       HiGh      |           Writes message marked as HIGH           | Matches expected |
|    Invalid parameter   |      qwerty     | Writes message marked as LOW + prints INFO string | Matches expected |

**Screenshots**

![Test Two - low](https://github.com/Dranevich/logger/blob/main/screens/TestTwo1.jpg)

**Default Severity Level: MEDIUM**
| Message severity level |   Input values  |                  Expected behaviour                  |  Real behaviour  |
|------------------------|-----------------|------------------------------------------------------|------------------|
|           LOW          |       low       |               Doesn't log this message               | Matches expected |
|          MEDIUM        |      medium     |          Writes message marked as MEDIUM             | Matches expected |
|           HIGH         |       high      |           Writes message marked as HIGH              | Matches expected |
|    Invalid parameter   |     <empty>     | Writes message marked as MEDIUM + prints INFO string | Matches expected |

**Screenshots**

![Test Two - medium](https://github.com/Dranevich/logger/blob/main/screens/TestTwo2.jpg)

**Default Severity Level: HIGH**
| Message severity level |   Input values  |                  Expected behaviour                |  Real behaviour  |
|------------------------|-----------------|----------------------------------------------------|------------------|
|           LOW          |       Low       |               Doesn't log this message             | Matches expected |
|          MEDIUM        |      medium     |               Doesn't log this message             | Matches expected |
|           HIGH         |       HiGh      |           Writes message marked as HIGH            | Matches expected |
|    Invalid parameter   |      qwerty     | Writes message marked as HIGH + prints INFO string | Matches expected |

**Screenshots**

![Test Two - high](https://github.com/Dranevich/logger/blob/main/screens/TestTwo3.jpg)

## Test 3: Socket Logging
**Test Description**

Using Netcat, listen own socket for incomming messages.

**Test Requrements**

Data recieved by a socket should be same as file output.

**Screenshots**

![Test Three](https://github.com/Dranevich/logger/blob/main/screens/TestThree.jpg)

# Module Log_stat_server
## Test 4: Socket Logging Analysis per N messages
**Test Description**

Trying to run log_stat_server with varius input parameters

**Test Requrements**

If parameters are correct, programm shoulg run succsessfully, overwise return error message.

|           Description          |                       Input values                    |                                Expected behaviour                                |  Real behaviour  |
|--------------------------------|-------------------------------------------------------|----------------------------------------------------------------------------------|------------------|
|          No parameters         |                    ./log_stat_server                  |                 Successful initialization, socket parameters are set to default  | Matches expected |
|        Only few parameters     |      ./log_stat_server --count 10 --timeout 300       | Successful initialization, selected parameters are applied, others set to default| Matches expected |
|        Invalid parameter       |              ./log_stat_server --qwerty 123           |                                   Error message                                  | Matches expected |
|    All parameters are correct  | ./log_stat_server --ip 0.0.0.0 --port 9000 --count 10 |                             Successful initialization                            | Matches expected |
|  Network target is unreachable | ./log_stat_server --ip 0.0.0.0 --port 9000 --count 10 |                           Socket creation error message                          | Matches expected |


**Screenshots**

![Test Four](https://github.com/Dranevich/logger/blob/main/screens/TestFour.jpg)

## Test 5: Socket Logging Analysis per N messages
**Test Description**

Checking socket statictics returned by log_stat_server every N messages.

**Test Requrements**

Program should print correct statistics after recieving every N=3 messages as well as show messages whemself.

**Screenshots**
![Test Five](https://github.com/Dranevich/logger/blob/main/screens/TestFive.jpg)

## Test 6: Socket Logging Analysis per T seconds
**Test Description**

Checking socket statictics returned by log_stat_server every T seconds.

**Test Requrements**

Program should print correct statistics after every T=3000 second as well as show messages whemself, if there were any new data.

**Screenshots**
![Test Five](https://github.com/Dranevich/logger/blob/main/screens/TestSix.jpg)

