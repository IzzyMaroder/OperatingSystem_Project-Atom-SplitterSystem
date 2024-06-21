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
    int thresh = 600, tot_atoms = 9000;
    int nscission = rand() % ((shmemory->stat.n_ofatoms - shmemory->stat.num_scories) - tot_atoms);
    printf("nscission %d\n", nscission);
    if(nscission > thresh) {
        wait_mutex(shmemory->conf.semId, STATE_SEM);
        shmemory->stat.flags = 1;
        increment_sem(shmemory->conf.semId, STATE_SEM);
    }
}


/* 
devo creare un rand con modulo la differenza tra la quantità di atomi creati e
la quantità di atomi max (9000) poi se il risulta è maggiore di una variabile thresh scelta a caso la scissione viene bloccata. 

*/