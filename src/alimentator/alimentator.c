#include "alimentator.h"

int * atompid;

void notifyatom(int counter) {
    int status;
    for (int j = 0; j < counter; j++) {
        kill(atompid[j], SIGTERM);
    }
    while(wait(&status) != -1) {
        printf("(ALIMENTATOR): Child terminated correctly\n");
    }
    exit(EXIT_SUCCESS);
}


void signal_handler() {
    notifyatom(shmemory->conf.conf_n_nuovi_atomi);
}

int main(int argc, char * argv[]){
    atompid = malloc(sizeof(int)*shmemory->conf.conf_step_alimentatore);
    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments alimentator.\n");
        exit(EXIT_FAILURE);
    }

    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGTERM, signal_handler);
    insert();

    exit(EXIT_SUCCESS);
}

void insert() {
    int i;
    char a_rand[20], memid_str[3*sizeof(int)+1];

    while(1) {
        nsleep(shmemory->conf.conf_step_alimentatore);
        for (i = 0; i < shmemory->conf.conf_n_nuovi_atomi; i++) {
            sprintf(a_rand, "%ld", (rand()%shmemory->conf.conf_n_atom_max+1) );
            sprintf(memid_str, "%d", shmemory->conf.memconf_id);

            atompid[i] = create_atoms(memid_str, a_rand);
        }
    }
}