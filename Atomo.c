#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int N_ATOM_MAX;

int main(int argc, char * argv[]) {

    if(argc < 2) { 
        fprintf(stderr, "Error: too/many arguments.\n");
        exit(EXIT_FAILURE);
    }
    
    N_ATOM_MAX = atoi(argv[1]);
    printf("N_ATOM_MAX: %d\n", N_ATOM_MAX);
}