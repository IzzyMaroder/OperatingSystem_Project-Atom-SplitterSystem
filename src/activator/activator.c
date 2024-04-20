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
#include <sys/msg.h>
#include <sys/sem.h>
#include "../lib/sharedmemory.h"
#include "semaphore.h"
#include "../lib/msgqueue.h"
#include "../lib/activator.h"

int N_ATOMI_INIT = 2;

int main(char * argv[], int argc) {
    struct msgbbufmio msg;
    //TODO: Farlo bene
    // while(read(file, &buffer, sizeof(buffer))) {
	// 	sscanf(buffer, "%*s %d %*s %d %*s %d",  &ENERGY_DEMAND, &N_ATOMI_INIT, &N_ATOM_MAX);
	// }
	// close(file);

    msgrcv(1, &msg, sizeof(int),0,0);
    printf("%d", msg.pid);
    //instanziare array
    tuple *tuplepid = malloc(sizeof(tuple) * (N_ATOMI_INIT * 2));
    //popolare array


}