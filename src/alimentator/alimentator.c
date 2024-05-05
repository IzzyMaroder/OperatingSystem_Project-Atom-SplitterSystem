#include "alimentator.h"

int activator_pid;

int main(int argc, char * argv[]){
    int i, status;
    char a_rand[20], memid_str[3*sizeof(int)+1];;
    
    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments alimentator.\n");
        exit(EXIT_FAILURE);
    }

    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        nsleep(shmemory->conf.conf_step_alimentatore);
        for (i = 0; i < shmemory->conf.conf_n_nuovi_atomi; i++) {
            sprintf(a_rand, "%ld", (rand()%shmemory->conf.conf_n_atom_max+1) );
            sprintf(memid_str, "%d", shmemory->conf.memconf_id);

            create_atoms(memid_str, a_rand);
        }
        // Inserire un segnale
        if(i == shmemory->conf.conf_n_nuovi_atomi) {
            while(wait(&status) != -1) {
                printf("(ALIMENTATOR): Child terminated correctly\n");
            }
            exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}
