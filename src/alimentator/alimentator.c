#include "alimentator.h"

int *atompid;
int counter;

void notifyatom(int counter) {
    int status;
    for (int j = 0; j < counter; j++) {
        if(kill(atompid[j], 0) == 0) {
            if(kill(atompid[j], SIGTERM) == -1) {
                printf("Error cannot send signal (ALIM) %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }
    }
    free(atompid);
    exit(EXIT_SUCCESS);
}


void signal_handler() {
    notifyatom(counter);
}

int main(int argc, char * argv[]){    
    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments alimentator.\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGTERM, signal_handler);

    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    insert();
}

void insert() {
    int i;
    char a_rand[20], memid_str[3*sizeof(int)+1];
    atompid = malloc(sizeof(int)*shmemory->conf.conf_n_nuovi_atomi);
    sprintf(memid_str, "%d", shmemory->conf.memconf_id);

    while(1) {
        nsleep(shmemory->conf.conf_step_alimentatore);
        counter+=shmemory->conf.conf_n_atomi_init;
        for (i = 0; i < shmemory->conf.conf_n_nuovi_atomi; i++) {
            sprintf(a_rand, "%ld", (rand()%shmemory->conf.conf_n_atom_max+1) );
            atompid[i] = create_atoms(memid_str, a_rand);
        }
    }
}