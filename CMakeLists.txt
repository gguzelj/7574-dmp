cmake_minimum_required(VERSION 3.9)
project(7574_dmp)

set(CMAKE_C_STANDARD 11)

add_executable(clientd
        src/clientd/clientd_init.c
        src/common/common.h
        src/clientd/connector/broker_connector.h
        src/clientd/connector/broker_connector.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/lib/msg.c
        src/lib/msg.h
        src/lib/shm.h
        src/lib/shm.c)

add_executable(clientd_i
        src/clientd/clientd_i.c
        src/common/common.h
        src/clientd/connector/broker_connector.h
        src/clientd/connector/broker_connector.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/lib/msg.h
        src/lib/msg.c
        src/lib/shm.h
        src/lib/shm.c
        src/clientd/id_mapper.h
        src/clientd/id_mapper.c)

add_executable(clientd_o
        src/clientd/clientd_o.c
        src/clientd/connector/broker_connector.h
        src/clientd/connector/broker_connector.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/msg.h
        src/lib/msg.c
        src/lib/shm.h
        src/lib/shm.c
        src/clientd/id_mapper.h
        src/clientd/id_mapper.c)

add_executable(aaaaa
        src/client/main.c
        src/clientd/client.h
        src/clientd/client.c
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

