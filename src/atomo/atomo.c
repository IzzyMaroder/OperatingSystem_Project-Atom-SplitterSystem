#include "atomo.h"

long N_ATOM, MIN_N_ATOMICO;

int main(int argc, char * argv[]) {
    struct shm * sharedmem;
    struct sembuf sops;
    struct msg msgq;
    if(argc < 4) {
        fprintf(stderr, "Error: too/many arguments.\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGUSR1, signal_handler);
    sharedmem = shmat(atoi(argv[1]), NULL, 0);
    if(sharedmem  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    // sops.sem_op = -1;
    // semop(atoi(argv[2]), &sops, 1);
    int sem_id = sharedmem->semId;
    int msgId = sharedmem->msgId;
    msgq.mtype = 1;
    msgq.pid = getpid();
    msgsnd(msgId, &msgq, sizeof(int), 0);
    N_ATOM = atoi(argv[2]);

    
    sleep(1);
}

void signal_handler(int signum) {
    printf("Ho ricevuto il segnale, %d\n", getpid());
}

int scission() {
    // int child_atom = fork();
    int n_atom_child;
    if(N_ATOM % 2 == 0) {
        N_ATOM = N_ATOM/2;
        n_atom_child = N_ATOM;
    }else {
        N_ATOM = N_ATOM/2;
        n_atom_child = N_ATOM+1;
    }
}