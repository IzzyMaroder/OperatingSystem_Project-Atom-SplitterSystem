#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include "../lib/sharedmemory/sharedmemory.h"
#include "../lib/semaphore.h"
#include "../lib/msgqueue.h"
#include "activator.h"

int N_ATOMI_INIT = 2;

int main(int argc, char * argv[]) {
    struct shm *shmemory;
    struct  msg msgqueu;
    int msgId;
    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }

    msgId = shmemory->msgId;
    msgrcv(msgId, &msgqueu, sizeof(int),0,0);
    printf("ATOMO PID: %d\n", msgqueu.pid);
    //instanziare array
    tuple *tuplepid = malloc(sizeof(tuple) * (N_ATOMI_INIT * 2));
    //popolare array


}