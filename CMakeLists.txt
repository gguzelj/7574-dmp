cmake_minimum_required(VERSION 3.9)
project(7574_dmp)

set(CMAKE_C_STANDARD 11)

add_executable(clientd
        src/client/client_deamon_initter.c
        src/common/common.h
        src/client/connector/broker_connector.h
        src/client/connector/broker_connector.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/lib/msg.c
        src/lib/msg.h)

add_executable(clientd_i
        src/client/clientd_i.c
        src/common/common.h
        src/client/connector/broker_connector.h
        src/client/connector/broker_connector.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/lib/msg.c
        src/lib/msg.h)

add_executable(clientd_o
        src/client/clientd_o.c
        src/client/connector/broker_connector.h
        src/client/connector/broker_connector.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/msg.c
        src/lib/msg.h)

add_executable(aaaaa
        src/test/main.c
        src/client/client.h
        src/client/client.c
        src/common/common.h
        src/lib/logger.h
        src/lib/logger.c
        src/lib/map.h
        src/lib/map.c
        src/lib/msg.c
        src/lib/msg.h)

add_executable(broker
        src/broker/main.c
        src/broker/broker.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/common/common.h)

