#include "alimentator.h"

#define ATOMO_NAME "./atomo"

// alimentator accede alla memoria condivisa, e da quella recupera step attivatore
int activator_pid;



int main(int argc, char * argv[]){
    struct shm * sharedmem;
    int i, status;
    char atom_rand[20];

    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments alimentator.\n");
        exit(EXIT_FAILURE);
    }

    sharedmem = shmat(atoi(argv[1]), NULL, 0);
    if(sharedmem  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }

    pid_t  *cpids_alim = malloc(sizeof(pid_t) * sharedmem->conf_n_nuovi_atomi);
    while(1) {
        nsleep(sharedmem->conf_step_alimentatore);
        for (i = 0; i < sharedmem->conf_n_nuovi_atomi; i++) {
            sprintf(atom_rand, "%d", ((int)(rand()%sharedmem->conf_n_atom_max+1)) );
            char * argr[4] = { ATOMO_NAME };
            char mem_str[3*sizeof(int)+1];
            sprintf(mem_str, "%d", sharedmem->memId);
            argr[1] = mem_str;
            argr[2] = atom_rand;
            argr[3] = NULL;
            switch (cpids_alim[i] = fork()) {
                case 0:
                    if(execve(ATOMO_NAME, argr, NULL) == -1) {
                        perror("Error: (ALIMENTATOR) failed to launch 'atom'.\n");
                        clean_all(sharedmem->memId);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case -1:
                    printf("Error: (ALIMENTATOR) falied fork to create atom process\n");
                    clean_all(sharedmem->memId);
                    exit(EXIT_FAILURE);
                default:
                    break;
            }
        }
        //Inserire un segnale
        if(i == sharedmem->conf_n_nuovi_atomi) {
            while(wait(&status) != -1) {
                printf("(ALIMENTATOR): Child terminated correctly\n");
            }
            exit(EXIT_SUCCESS);
        }
    }
    free(cpids_alim);
    exit(EXIT_SUCCESS);
}
