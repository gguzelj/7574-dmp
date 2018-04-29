#ifndef _SEMAFORO_H_
#define _SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#include "logger.h"
#include "resources.h"


int create_sem(int identif) {
	key_t clave;
	clave = ftok(DIRECTORY, identif);
	return (semget(clave, 1, IPC_CREAT | IPC_EXCL | 0660));
	/* da error si ya existe */
}

int get_sem(int identif) {
	key_t clave;
	clave = ftok(DIRECTORY, identif);
	return (semget(clave, 1, 0660));
}

int init_sem(int semid, int val) {
	union semun {
		int val;
		/* Value for SETVAL */
		struct semid_ds *buf;
		/* Buffer for IPC_STAT, IPC_SET */
		unsigned short *array; /* Array for GETALL, SETALL */
		struct seminfo *__buf; /* Buffer for IPC_INFO(Linux specific)*/
	} arg;
	arg.val = val;
	return (semctl(semid, 0, SETVAL, arg));
}

/**
 * WAIT
 */
int p(int semid) {
	struct sembuf oper;
	oper.sem_num = 0;
	/* nro. de semáforo del set */
	oper.sem_op = -1;
	/* p(sem) */
	oper.sem_flg = 0;

	if(semop(semid, &oper, 1)!=0){
	  safeperror("can not release sem");
	  exit(-1);
	}

	return 1;
}

/**
 * SIGNAL
 */
int v(int semid) {
	struct sembuf oper;
	oper.sem_num = 0;
	/* nro. de semáforo */
	oper.sem_op = 1;
	/* v(sem) */
	oper.sem_flg = 0;
	if(semop(semid, &oper, 1)!=0){
	  safeperror("can not acquire sem");
	  exit(-1);
	}
	return 1;
}

int remove_sem(int semid) {
	return (semctl(semid, 0, IPC_RMID, (struct semid_ds *) 0));
}

#endif /* _SEMAFORO_H_ */
