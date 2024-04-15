#include <stdio.h>
#include <stdlib.h>

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

    printf("Shared memory passed all init phase.\n");

}