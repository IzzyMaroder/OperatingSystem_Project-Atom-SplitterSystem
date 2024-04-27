#define _GNU_SOURCE
#include "activator.h"

int N_ATOMI_INIT = 2;

int main(int argc, char * argv[]) {
    struct shm *shmemory;
    struct  msg msgqueu;
    int msgId;
    int counter = 0;
    int atom;
    srand(getpid());
    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }

    msgId = shmemory->msgId;

    //instanziare array
    tuple *tuplepid = malloc(sizeof(tuple) * (N_ATOMI_INIT * 2));
    //popolare array

    sleep(1);
    while ((msgrcv(msgId, &msgqueu, sizeof(int), 0, IPC_NOWAIT) != -1)) {
        tuplepid[counter].pid = msgqueu.pid;
        tuplepid[counter].alive = true;
        counter++;
    }

    do {

        atom = rand() % counter + 1;

    } while (tuplepid[atom].alive == false);

    kill(tuplepid[atom].pid, SIGUSR1);

    msgctl (msgId, IPC_RMID , NULL);
}