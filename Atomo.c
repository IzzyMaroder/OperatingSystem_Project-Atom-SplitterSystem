#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "sharedmemory.h"

int N_ATOM_MAX;
struct shm * sharedmem;

int main(int argc, char * argv[]) {

    if(argc < 3) { 
        fprintf(stderr, "Error: too/many arguments.\n");
        exit(EXIT_FAILURE);
    }

    sharedmem = shmat(atoi(argv[1]), NULL, 0);
    printf("Memoria da riportare %d\n",atoi(argv[1]));
    if(sharedmem  == NULL) {
        fprintf(stderr, "Error: sharedmem in atomo.\n");
        exit(EXIT_FAILURE);
    }
    printf("TOT_ACTIVATIONS: %d\n", sharedmem->TOT_ACTIVATIONS);

    N_ATOM_MAX = atoi(argv[2]);
    printf("N_ATOM_MAX: %d\n", N_ATOM_MAX);

    clean_mem(sharedmem);
}