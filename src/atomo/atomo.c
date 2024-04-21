#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "../lib/sharedmemory/sharedmemory.h"
#include "../lib/semaphore.h"
#include "../lib/msgqueue.h"

int N_ATOM_MAX;

int main(int argc, char * argv[]) {
    struct shm * sharedmem;
    struct sembuf sops;
    struct msg msgq;
    if(argc < 3) {
        fprintf(stderr, "Error: too/many arguments.\n");
        exit(EXIT_FAILURE);
    }

    sharedmem = shmat(atoi(argv[1]), NULL, 0);
    if(sharedmem  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    // sops.sem_op = -1;
    // semop(atoi(argv[2]), &sops, 1);
    int msgId = sharedmem->msgId;
    msgq.mtype = 1;
    msgq.pid = getpid();
    msgsnd(msgId, &msgq, sizeof(int), 0);
    msgctl (msgId, IPC_RMID , NULL);

    N_ATOM_MAX = atoi(argv[2]);

    clean_mem(sharedmem);
}