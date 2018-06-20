cmake_minimum_required(VERSION 3.5)
project(7574_dmp)

set(CMAKE_C_STANDARD 11)

add_executable(clientd
        src/clientd/clientd_init.c
        src/common/common.h
        src/common/broker_connector/broker_connector.h
        src/common/broker_connector/broker_connector.c
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
        src/common/broker_connector/broker_connector.h
        src/common/broker_connector/broker_connector.c
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
        src/common/broker_connector/broker_connector.h
        src/common/broker_connector/broker_connector.c
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
        src/clientd/id_mapper/id_mapper.c src/broker/db/db.h src/broker/db/db.c)

add_executable(broker
        src/broker/broker.h
        src/broker/broker.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/msg.h
        src/lib/msg.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/log/logger.h
        src/lib/log/logger.c
        src/common/common.h)

add_executable(next_broker_connector
        src/broker/broker_chain/next_broker_connector.c
        src/broker/broker_chain/next_broker_connector.h
        src/common/common.h
        src/common/broker_connector/broker_connector.h
        src/common/broker_connector/broker_connector.c
        src/lib/msg.h
        src/lib/msg.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c)

add_executable(broker_i
        src/broker/receiver/broker_receiver.h
        src/broker/receiver/broker_receiver.c
        src/common/common.h
        src/lib/msg.h
        src/lib/msg.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c)

add_executable(broker_o
        src/broker/responder/broker_responder.h
        src/broker/responder/broker_responder.c
        src/common/common.h
        src/lib/msg.h
        src/lib/msg.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c)

add_executable(worker
        src/broker/worker/worker.h
        src/broker/worker/worker.c
        src/common/common.h
        src/broker/db/db.h
        src/broker/db/db.c
        src/lib/msg.h
        src/lib/msg.c
        src/lib/logger.h
        src/lib/logger.c
        src/lib/lockfile/lockfile.h
        src/lib/lockfile/lockfile.c
        src/lib/log/logger.h
        src/lib/log/logger.c)


