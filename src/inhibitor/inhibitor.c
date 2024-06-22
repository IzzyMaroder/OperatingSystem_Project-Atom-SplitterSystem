#include "inhibitor.h"


int main(int argc, char *argv[]) {

    if((shmemory = shmat(atoi(argv[1]), NULL, 0)) == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_FAILURE);
    }
    doscission();
}

void signal_handler() {
    exit(EXIT_SUCCESS);
}

void doscission() {
    while(1){
        int thresh = 500, tot_atoms = 9000;
        int nscission = rand() % (tot_atoms - shmemory->stat.n_ofatoms);
        if(nscission < thresh) {
            wait_mutex(shmemory->conf.semId, STATE_SEM);
            shmemory->stat.flags = 1;
            increment_sem(shmemory->conf.semId, STATE_SEM);
        } else {
            printf(" ENERGY %ld\n", ENERGY_EXPLODE_THRESHOLD);
            int excess_energy = (shmemory->stat.energy_produced - shmemory->stat.energy_consumed) - ENERGY_EXPLODE_THRESHOLD;
            wait_mutex(shmemory->conf.semId, STATE_SEM);
            shmemory->stat.energy_to_remove = (excess_energy / (shmemory->stat.energy_produced - shmemory->stat.energy_consumed));
            shmemory->stat.flags = 0;
            increment_sem(shmemory->conf.semId, STATE_SEM);
        }
    }
}


/* 
devo creare un rand con modulo la differenza tra la quantità di atomi creati e
la quantità di atomi max (9000) poi se il risulta è maggiore di una variabile thresh scelta a caso la scissione viene bloccata. 

SECONDA PARTE:
Se la fissione non è stata inibita, togli una quantità di energia liberata per prevenire l'explode
(togli un valore in proporzione alla differenza tra current energy e explode thresh.)


*/