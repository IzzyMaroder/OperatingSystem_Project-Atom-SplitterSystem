#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif


int ENERGY_DEMAND;
int N_ATOMI_INIT;


int input_file(char * pathname) {
	int file;

	if((file = open(pathname, O_RDONLY)) == -1) {
		perror("failed to open file!\n");
		close(file);
		exit(EXIT_FAILURE);

	}
	char buffer[100];
	while(read(file, &buffer, sizeof(buffer))) {
		sscanf(buffer, "%*s %d %*s %d", &ENERGY_DEMAND, &N_ATOMI_INIT);
		printf("ATOMI %d\n", N_ATOMI_INIT);
		printf("ENERGY %d\n", ENERGY_DEMAND);
	}
	close(file);
}

int main() {
	input_file("init_file.txt");
}
