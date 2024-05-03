#include "alimentator.h"

int activator_pid;

int main(int argc, char * argv[]){
    struct shmConf * shconfmem;
    int i, status;
    char a_rand[20], memid_str[3*sizeof(int)+1];;

    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments alimentator.\n");
        exit(EXIT_FAILURE);
    }

    shconfmem = shmat(atoi(argv[1]), NULL, 0);
    if(shconfmem  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        nsleep(shconfmem->conf_step_alimentatore);
        for (i = 0; i < shconfmem->conf_n_nuovi_atomi; i++) {
            sprintf(a_rand, "%ld", (rand()%shconfmem->conf_n_atom_max+1) );
            sprintf(memid_str, "%d", shconfmem->memId);

            create_atoms(memid_str, a_rand);
        }
        //Inserire un segnale
        if(i == shconfmem->conf_n_nuovi_atomi) {
            while(wait(&status) != -1) {
                printf("(ALIMENTATOR): Child terminated correctly\n");
            }
            exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}
