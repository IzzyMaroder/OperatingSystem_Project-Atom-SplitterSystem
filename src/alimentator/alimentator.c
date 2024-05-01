#include <stdio.h>
#include <unistd.h>
#include "../lib/sharedmemory/sharedmemory.h"

#define ATOMO_NAME "./atomo"

struct shm * sharedmem;
long STEP_ATTIVATORE,N_ATOM_MAX,N_NUOVI_ATOMI;
// alimentator accede alla memoria condivisa, e da quella recupera step attivatore
int activator_pid;
void atom_generation(int argc, char * argv[]){
    (void) argc;
    switch (activator_pid = fork()) {
    //child process
        case 0:
            if(execve(ATOMO_NAME, argv, NULL) == -1) {
                perror("Error: failed to launch 'alimentator'.\n");
                exit(EXIT_FAILURE);
            }
            break;
        //error
        case -1:
            printf("Error: falied fork to create alimentator process");
            exit(EXIT_FAILURE);
        default:
            break;
    }
}


int main(int argc, char * argv[]){
    int n_atom_rand;
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
    STEP_ATTIVATORE = sharedmem->conf_step_attivatore;
    N_ATOM_MAX = sharedmem->conf_n_atom_max;
    N_NUOVI_ATOMI = sharedmem->conf_n_nuovi_atomi;
    int activator_pid;
    for (int i = 0; i<N_NUOVI_ATOMI; i++) {
        n_atom_rand = rand()%N_ATOM_MAX+1;
		sprintf(atom_rand, "%d", n_atom_rand);
        char * argr[4] = { ATOMO_NAME };
        char mem_str[3*sizeof(int)+1];
        sprintf(mem_str, "%d", sharedmem->memId);
        argr[1] = mem_str;
        argr[2] = atom_rand;
        argr[3] = NULL;
        atom_generation(argc, argr);
    }
}
