#define _GNU_SOURCE
#include "../sharedmemory/sharedmemory.h"
#include "general.h"

void clean_all(int mem_id) {
    struct shm *shmemory;
    shmemory = shmat(mem_id, NULL, 0);

    msgctl(shmemory->msgId, IPC_RMID, NULL);
    semctl(shmemory->semId, 0, IPC_RMID);
    shmdt(shmemory);
    shmctl( mem_id, IPC_RMID , NULL);
}