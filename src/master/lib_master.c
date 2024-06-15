#include "master.h"

long ENERGY_DEMAND, N_ATOMI_INIT, N_ATOM_MAX, MIN_N_ATOMICO, STEP_ATTIVATORE, N_NUOVI_ATOMI, STEP_ALIMENTATORE, SIM_DURATION, ENERGY_EXPLODE_THRESHOLD;

void input_file(char * pathname) {
	int file;

	if((file = open(pathname, O_RDONLY)) == -1) {
		perror("failed to open file!\n");
		close(file);
		exit(EXIT_FAILURE);

	}
	char buffer[600];
	while(read(file, &buffer, sizeof(buffer))) {
		sscanf(buffer, "%*s %ld %*s %ld %*s %ld %*s %ld %*s %ld %*s %ld %*s %ld %*s %ld %*s %ld", &ENERGY_DEMAND, &N_ATOMI_INIT, &N_ATOM_MAX, &MIN_N_ATOMICO, &STEP_ATTIVATORE, &N_NUOVI_ATOMI, &STEP_ALIMENTATORE, &SIM_DURATION, &ENERGY_EXPLODE_THRESHOLD);
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

void printstat(int sem_id, struct shmStat* snap) {
    printf("---------------------------------------------\n");
    printf("SCORIES: %d, (%d)\n",shmemory->stat.num_scories,shmemory->stat.num_scories - snap->num_scories);
    printf("ENERGY PRODUCED: %d\n",shmemory->stat.energy_produced);
    printf("ENERGY CONSUMED: %d\n",shmemory->stat.energy_consumed);
    printf("NUMBER OF SCISSIONS: %d\n",shmemory->stat.num_scissions);
    printf("NUMBER OF ACTIVATION: %d\n",shmemory->stat.num_activations);

}
