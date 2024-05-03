#define _GNU_SOURCE
#include "general.h"
#include "../sharedmemory/sharedmemory.h"
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


void create_atoms(char * memid_str, char * a_rand) {
    int cpids;
    char * argq[4] = { ATOMO_NAME };
		argq[1] = memid_str;
		argq[2] = a_rand;
		argq[3] = NULL;
		switch(cpids = fork()) {
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