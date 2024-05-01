#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../lib/sharedmemory/sharedmemory.h"

#define ATOMO_NAME "./atomo"

struct shm * sharedmem;
long STEP_ATTIVATORE,N_ATOM_MAX,N_NUOVI_ATOMI;
// alimentator accede alla memoria condivisa, e da quella recupera step attivatore
int activator_pid;



int main(int argc, char * argv[]){
    int n_atom_rand, status;
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
    int mem_id = sharedmem->memId;
    STEP_ATTIVATORE = sharedmem->conf_step_attivatore;
    N_ATOM_MAX = sharedmem->conf_n_atom_max;
    N_NUOVI_ATOMI = sharedmem->conf_n_nuovi_atomi;
    printf("argv[1]: %d\nmem_id: %d\nSTEP ATTIVATORE: %ld\nN_ATOM_MAX:%ld\nN_NUOVI_ATOMI: %ld\n",atoi(argv[1]),mem_id,STEP_ATTIVATORE,N_ATOM_MAX,N_NUOVI_ATOMI);
    pid_t  *cpids_alim = malloc(sizeof(pid_t) * N_NUOVI_ATOMI);
    for (int i = 0; i<N_NUOVI_ATOMI; i++) {
        n_atom_rand = rand()%N_ATOM_MAX+1;
		sprintf(atom_rand, "%d", n_atom_rand);
        char * argr[4] = { ATOMO_NAME };
        char mem_str[3*sizeof(int)+1];
        sprintf(mem_str, "%d", mem_id);
        argr[1] = mem_str;
        argr[2] = atom_rand;
        argr[3] = NULL;
        switch (cpids_alim[i] = fork()) {
        //child process
            case 0:
                if(execve(ATOMO_NAME, argr, NULL) == -1) {
                    perror("Error: (ALIMENTATOR) failed to launch 'atom'.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            //error
            case -1:
                printf("Error: (ALIMENTATOR) falied fork to create atom process\n");
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
    while(wait(&status) != -1) {
        printf("(ALIMENTATOR): Child terminated correctly\n");
    }
    exit(EXIT_SUCCESS);
    free(cpids_alim);
}
