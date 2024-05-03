#include <unistd.h>
#define _GNU_SOURCE
#include "../sharedmemory/sharedmemory.h"
#include "general.h"

struct shmConf *shmemory;

void clean_all(int mem_id) {
    shmemory = shmat(mem_id, NULL, 0);

    msgctl(shmemory->msgId, IPC_RMID, NULL);
    semctl(shmemory->semId, 0, IPC_RMID);
    shmdt(shmemory);
    shmctl( mem_id, IPC_RMID , NULL);
}

void nsleep(long step) {
    struct timespec reqtime, retime;
    reqtime.tv_sec = 0;
    reqtime.tv_nsec = step;
    if((nanosleep(&reqtime , &retime) != 0) && errno == EINTR) {
      reqtime = retime;
    }
}

void wait_mutex(int sem_id, unsigned short sem_num) {
  struct sembuf sops = {sem_num, -1, 0};
  if(semop(sem_id, &sops, 1) == -1) {
    printf("Error: Semaphore\n");
  }
}

void increment_sem(int sem_id, unsigned short sem_num) {
  struct sembuf sops = {sem_num, 1, 0};
  if(semop(sem_id, &sops, 1) == -1) {
    printf("Error: Semaphore\n");
  }
}


void create_atoms(pid_t * cpids, char * memid_str) {
  char atom_rand[20];
  int i;
  for(i = 0; i < shmemory->conf_n_atomi_init;  i++) {
		sprintf(atom_rand, "%d", ((int)(rand()%shmemory->conf_n_atom_max+1)));
    char * argq[4] = { ATOMO_NAME };
		argq[1] = memid_str;
		argq[2] = atom_rand;
		argq[3] = NULL;
		switch(cpids[i] = fork()) {
			case -1:
				fprintf(stderr,"Error: failed to fork.\n");
				clean_all(shmemory->memId);
        exit(EXIT_FAILURE);
			case 0:
				if(execve(ATOMO_NAME, argq, NULL) == -1) {
					perror("Error: failed to launch 'atomo'.\n");
					clean_all(shmemory->memId);
          exit(EXIT_FAILURE);
				}
			default:
				break;
		}
	}
}

void create_process(char * memid_str, char * name) {
  int process_pid;
  char * args[3] = { name };
  args[1] = memid_str;
  switch (process_pid = fork()) {
    case 0:
        if(execve(name, args, NULL) == -1) {
            perror("Error: failed to launch PROCESS.\n");
            clean_all(shmemory->memId);
            exit(EXIT_FAILURE);
        }
        break;
    case -1:
        printf("Error: falied fork to create alimentator process\n");
        clean_all(shmemory->memId);
        exit(EXIT_FAILURE);
    default:
        break;
    }
}