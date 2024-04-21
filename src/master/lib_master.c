#include "master.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

struct shm *shmemory;
struct sembuf sops;
struct msg msgq;
long ENERGY_DEMAND, N_ATOMI_INIT, N_ATOM_MAX;

void input_file(char * pathname) {
	int file;

	if((file = open(pathname, O_RDONLY)) == -1) {
		perror("failed to open file!\n");
		close(file);
		exit(EXIT_FAILURE);

	}
	char buffer[100];
	while(read(file, &buffer, sizeof(buffer))) {
		sscanf(buffer, "%*s %ld %*s %ld %*s %ld", &ENERGY_DEMAND, &N_ATOMI_INIT, &N_ATOM_MAX);
	}
	close(file);
}

int mem_init() {
    int memId = shmget(IPC_PRIVATE, sizeof(struct shm), 0600 | IPC_CREAT);

    if(memId == -1) {
        fprintf(stderr, "Error: failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    printf("Shared memory passed all init phase.\n");

    return memId;
}

int msg_init() {
    int msgId = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);

    if(msgId == -1) {
        fprintf(stderr, "Error: failed to get msg queue.\n");
        exit(EXIT_FAILURE);
    }
	printf("msg queue passed all init phase.\n");

    return msgId;
}

int sem_init(int sem_num){
	int sem_id = semget(IPC_PRIVATE, sem_num, 0600 | IPC_CREAT);
	if (sem_id == -1) {
		fprintf(stderr, "Error: failed to get semaphore array.\n");
        exit(EXIT_FAILURE);
	}
	printf("semaphore passed all init phase.\n");

	return sem_id;
}
