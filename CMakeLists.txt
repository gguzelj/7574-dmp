cmake_minimum_required(VERSION 3.9)
project(7574_dmp)

set(CMAKE_C_STANDARD 11)

add_executable(client
        src/client/main.c
        src/common/common.h
        src/client/broker_connector.h
        src/client/broker_connector.c
        src/client/client_service.h
        src/client/client_service.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/msg.c
        src/lib/msg.h)

add_executable(aaaaa
        src/test/main.c
        src/common/common.h
        src/client/broker_connector.h
        src/client/broker_connector.c
        src/client/client_service.h
        src/client/client_service.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/msg.c
        src/lib/msg.h)

add_executable(broker src/broker/main.c src/broker/broker.c src/common/common.h)

