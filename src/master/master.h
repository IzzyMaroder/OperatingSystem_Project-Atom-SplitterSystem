#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "../lib/sharedmemory/sharedmemory.h"
#include "../lib/semaphore.h"
#include "../lib/msgqueue.h"


extern long ENERGY_DEMAND, N_ATOMI_INIT, N_ATOM_MAX, MIN_N_ATOMICO, STEP_ATTIVATORE, N_NUOVI_ATOMI;


void input_file(char * pathname);
int mem_init();
int msg_init();
void start_simulation();
int sem_init(int sem_num);
