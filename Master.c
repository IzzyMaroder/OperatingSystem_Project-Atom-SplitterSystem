#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int ENERGY_DEMAND;
int N_ATOMI_INIT;


int input_file(char * pathname) {
	int file = open(pathname, O_RDONLY);

	if(file == -1) { 
		perror("failed to open file!\n");
		close(file);
		exit(EXIT_FAILURE);
	}

	lseek(file, 14, SEEK_SET);
	char * buf = malloc(1);
	read(file, buf, 1);
	ENERGY_DEMAND = atoi(buf);
	
	lseek(file, 28, SEEK_CUR);
	read(file, buf, 1);
	N_ATOMI_INIT = atoi(buf);
	
	close(file);
}

int main() {
	input_file("init_file.txt");
}
