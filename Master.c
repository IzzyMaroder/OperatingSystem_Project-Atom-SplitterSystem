#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sharedmemory.h"

#define ATOMO_NAME "./atomo"

int ENERGY_DEMAND;
int N_ATOMI_INIT;
int N_ATOM_MAX;
struct shm *sharedmem;

int input_file(char * pathname) {
	int file;

	if((file = open(pathname, O_RDONLY)) == -1) {
		perror("failed to open file!\n");
		close(file);
		exit(EXIT_FAILURE);

	}
	char buffer[100];
	while(read(file, &buffer, sizeof(buffer))) {
		sscanf(buffer, "%*s %d %*s %d %*s %d", &ENERGY_DEMAND, &N_ATOMI_INIT, &N_ATOM_MAX);
	}
	close(file);
}

void atoms_create() {
	int i, status, n_atom_rand, memid;
	char buf[20], memid_str[3*sizeof(memid)+1];
	char * args[4] = { ATOMO_NAME };
	pid_t * cpids;

	cpids = malloc(sizeof(cpids)*N_ATOMI_INIT);
	memid = sharedmem->memId;
	sprintf(memid_str, "%d", memid);
	// int memid =0;

	for(i = 0; i < N_ATOMI_INIT;  i++) {

		n_atom_rand = rand()%N_ATOM_MAX+1;
		sprintf(buf, "%d", n_atom_rand);
		args[1] = memid_str;
		args[2] = buf;
		args[3] = NULL;
		switch(cpids[i] = fork()) {
			case -1:
				fprintf(stderr,"Error: failed to fork.\n");
				exit(EXIT_FAILURE);
			case 0:
				if(execve(ATOMO_NAME, args, NULL) == -1) {
					perror("Error: failed to launch 'atomo'.\n");
					exit(EXIT_FAILURE);
				}
			default:
				break;
		}
	}
	while(wait(&status) != -1) {
		printf("Figlio terminato correttamente.\n");

	}
	free(cpids);
	exit(EXIT_SUCCESS);
}

void start_simulation() {
	sharedmem = mem_init();
	// print_mem(shmemory);
	atoms_create();
	clean_mem(sharedmem);
}

int main() {
	srand(getpid());
	input_file("init_file.txt");
	start_simulation();
}
