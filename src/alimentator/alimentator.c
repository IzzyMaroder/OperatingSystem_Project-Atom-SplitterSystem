#include "alimentator.h"
#include <unistd.h>

int *atompid;
int counter;


void signal_handler() {
    
}

int main(int argc, char * argv[]){    
    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments alimentator.\n");
        exit(EXIT_FAILURE);
    }


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
        if(shmemory->stat.flags == 0){
            counter+=shmemory->conf.conf_n_atomi_init;
            for (i = 0; i < shmemory->conf.conf_n_nuovi_atomi; i++) {
                sprintf(a_rand, "%ld", (rand()%shmemory->conf.conf_n_atom_max+1) );
                atompid[i] = create_atoms(memid_str, a_rand);
                if (atompid[i] == -1) {
                    pause();
                }
            }
        }
    }
}