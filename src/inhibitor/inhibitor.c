#include "inhibitor.h"

#define TOLLERANCE (shmemory->conf.conf_energy_thresh / 100)

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
        nsleep(99999999);
        int thresh = 500, tot_atoms = 9000;
        int nscission = rand() % (tot_atoms - shmemory->stat.n_ofatoms);
        if(nscission < thresh) {
            wait_mutex(shmemory->conf.semId, STATE_SEM);
            shmemory->stat.flags = 1;
            increment_sem(shmemory->conf.semId, STATE_SEM);
        } else {
            //Energia fino a questo momento
            double delta_energy_th = shmemory->conf.conf_energy_thresh - (shmemory->stat.energy_produced - shmemory->stat.energy_consumed);
            //printf(" SONO QUIIIIII %d, PRODOTT %d\n", excess_energy, (shmemory->stat.energy_produced - shmemory->stat.energy_consumed));
            wait_mutex(shmemory->conf.semId, STATE_SEM);
            shmemory->stat.flags = 0;
            shmemory->stat.energy_to_remove = (((shmemory->stat.energy_produced - shmemory->stat.energy_consumed) + TOLLERANCE) / delta_energy_th);
            printf("REMOVE %f\n", shmemory->stat.energy_to_remove);
            increment_sem(shmemory->conf.semId, STATE_SEM);
        }
    }
}