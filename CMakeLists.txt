cmake_minimum_required(VERSION 3.9)
project(7574_dmp C)

set(CMAKE_C_STANDARD 11)

add_executable(client src/client/main.c src/client/client.c)
add_executable(broker src/broker/main.c src/broker/broker.c)