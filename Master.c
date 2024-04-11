#include <stdio.h>
#include <stdlib.h>
#include <errno.h>





int main() {


}



int input_file(char * pathname) {
	int file = open(pathname, O_RDONLY);
	if(file == -1) { perror("failed to open file!\n"); exit(EXIT_FAILURE); }
	lseek(file, 14, SEEK_SET);
	char * buf = malloc(1);
	read(file, buf, 1);
	ENERGY_DEMAND = atoi(buf);
	
	lseek(file, 14, SEEK_CUR);
	read(file, buf, 1);
	N_ATOMS_INIT = atoi(buf);

	close(file);	
}
