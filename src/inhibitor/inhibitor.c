#include "inhibitor.h"


int main(int argc, char *argv[]) {

    if((shmemory = shmat(atoi(argv[1]), NULL, 0)) == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_FAILURE);
    }
    doscission();

    // pause();
}

void signal_handler() {
    exit(EXIT_SUCCESS);
}

void doscission() {
    while(1){
        sleep(3);
        int thresh = 500, tot_atoms = 9000;
        int nscission = rand() % (tot_atoms - shmemory->stat.n_ofatoms);
        // printf("nscission %d\n", nscission);
        if(nscission > thresh) {
            wait_mutex(shmemory->conf.semId, STATE_SEM);
            shmemory->stat.flags = 1;
            increment_sem(shmemory->conf.semId, STATE_SEM);
        } else {
            wait_mutex(shmemory->conf.semId, STATE_SEM);
            shmemory->stat.flags = 0;
            increment_sem(shmemory->conf.semId, STATE_SEM);
        }
    }
}


/* 
devo creare un rand con modulo la differenza tra la quantità di atomi creati e
la quantità di atomi max (9000) poi se il risulta è maggiore di una variabile thresh scelta a caso la scissione viene bloccata. 

*/