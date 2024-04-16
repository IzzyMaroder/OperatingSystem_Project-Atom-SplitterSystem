#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sharedmemory.h"

struct shm* mem_init() {
    struct shm * datap;

    int memId = shmget(IPC_PRIVATE, sizeof(struct shm), 0600 | IPC_CREAT);

    if(memId == -1) {
        fprintf(stderr, "Error: failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    datap = shmat(memId, NULL, 0);
    if(datap == NULL) {
        fprintf(stderr, "Error: failed to attach memory.\n");
        exit(EXIT_FAILURE);
    }

    shmctl(memId, IPC_RMID, NULL);

    datap->CURRENT_ENERGY = 0;

    datap->TOT_ACTIVATIONS = 100;
    datap->ACTIVATIONS_PER_SEC = 0;
    
    datap->TOT_SCISSIONS = 0;
    datap->SCISSIONS_PER_SEC = 0;
    
    datap->TOT_PRODUCED_ENERGY = 0;
    datap->PRODUCED_ENERGY_PER_SEC = 0;
    
    datap->TOT_CONSUMED_ENERGY = 0;
    datap->CONSUMED_ENERGY_PER_SEC = 0;
    
    datap->TOT_SCORIE = 0;
    datap->SCORIE_PER_SEC = 0;
    
    datap->ABSORBED_ENERGY = 0;
    datap->memId = memId;
    
    printf("Shared memory passed all init phase.\n");

    return datap;

}

void print_mem(struct shm * datap) {
    datap->TOT_ACTIVATIONS = datap->TOT_ACTIVATIONS+ datap->ACTIVATIONS_PER_SEC;

    printf("Total activations: %d\n", datap->TOT_ACTIVATIONS);

}

void clean_mem(struct shm * datap) {
    shmdt(datap);
}