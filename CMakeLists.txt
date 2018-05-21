cmake_minimum_required(VERSION 3.9)
project(7574_dmp)

set(CMAKE_C_STANDARD 11)

add_executable(clientd
        src/clientd/clientd_init.c
        src/common/common.h
        src/clientd/broker_connector/broker_connector.h
        src/clientd/broker_connector/broker_connector.c
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
        src/clientd/receiver/clientd_i.c
        src/clientd/broker_connector/broker_connector.h
        src/clientd/broker_connector/broker_connector.c
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
        src/clientd/id_mapper/id_mapper.h
        src/clientd/id_mapper/id_mapper.c)

add_executable(clientd_o
        src/clientd/responder/clientd_o.c
        src/clientd/broker_connector/broker_connector.h
        src/clientd/broker_connector/broker_connector.c
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
        src/clientd/id_mapper/id_mapper.h
        src/clientd/id_mapper/id_mapper.c)

add_executable(aaaaa
        src/client/main.c
        src/clientd/client.h
        src/clientd/client.c
        src/common/common.h
        src/lib/logger.h
        src/lib/logger.c
        src/lib/msg.c
        src/lib/msg.h
        src/lib/shm.h
        src/lib/shm.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/clientd/id_mapper/id_mapper.h
        src/clientd/id_mapper/id_mapper.c)

add_executable(broker
        src/broker/broker.h
        src/broker/broker.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/common/common.h)

add_executable(broker_i
        src/broker/receiver/broker_receiver.h
        src/broker/receiver/broker_receiver.c
        src/common/common.h
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c)

add_executable(broker_o
        src/broker/responder/broker_responder.h
        src/broker/responder/broker_responder.c
        src/common/common.h
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c)


