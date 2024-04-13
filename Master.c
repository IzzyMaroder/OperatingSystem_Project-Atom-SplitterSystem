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
	/* START DEBUG */
	if(ENERGY_DEMAND) {
		printf("readed ENERGY_DEMAND %d\n", ENERGY_DEMAND);
	} else {
		fprintf(stderr,"Error: ENERGY_DEMAND is null.\n");
		close(file);
		exit(EXIT_FAILURE);
	}
	/* END DEBUG */

	lseek(file, 14, SEEK_CUR);
	read(file, buf, 1);
	N_ATOMI_INIT = atoi(buf);
	/* START DEBUG */
	if(N_ATOMI_INIT) {
		printf("readed N_ATOMI_INIT %d\n", N_ATOMI_INIT);
	} else {
		fprintf(stderr, "Error: N_ATOMI_INIT is null.\n");
		close(file);
		exit(EXIT_FAILURE);
	}
	/* END DEBUG */
	close(file);
}

int main() {
	input_file("init_file.txt");
}
