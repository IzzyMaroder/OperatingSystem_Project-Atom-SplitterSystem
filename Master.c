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
	FILE * file;

	if((file = fopen(pathname, "r")) == NULL ) { 
		perror("failed to open file!\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	char buffer[100];
	fseek(file, 14, SEEK_CUR);
	fscanf(file, "%d", &ENERGY_DEMAND);
	printf("%d\n", ENERGY_DEMAND);
	while(fgets(buffer, sizeof(buffer), file) != NULL) {
		fseek(file, 14, SEEK_CUR);
		fscanf(file, "%d", &N_ATOMI_INIT);
		printf("%d\n", N_ATOMI_INIT);

	}
	
	fclose(file);
}

int main() {
	input_file("init_file.txt");
}
