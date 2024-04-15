#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char * argv[]) {
    if(argc < 2) { fprintf(stderr, "Error: too/many arguments.\n");  exit(EXIT_FAILURE);}

    fprintf(stderr,"Atomo launched successfully!\n");
}