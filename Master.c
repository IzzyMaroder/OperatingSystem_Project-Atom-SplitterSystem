#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define _GNU_SOURCE


int ENERGY_DEMAND;
int N_ATOMI_INIT;
int N_ATOM_MAX;

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
	int i, status;
	int n_atom_rand;
	char buf[20];
	pid_t * cpids;

	cpids = malloc(sizeof(cpids)*N_ATOMI_INIT);
	for(i = 0; i < N_ATOMI_INIT;  i++) {
		n_atom_rand = rand()%N_ATOM_MAX+1;
		sprintf(buf, "%d", n_atom_rand);
		char * args[] = { "", buf, NULL };
		switch(cpids[i] = fork()) {
			case -1:
				fprintf(stderr,"Error: failed to fork.\n");
				exit(EXIT_FAILURE);
			case 0:
				if(execve("./atomo", args, NULL) == -1) {
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

int main() {
	srand(getpid());
	input_file("init_file.txt");
	atoms_create();
}
