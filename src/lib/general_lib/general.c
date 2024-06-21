#include "../../common.h"
#include <signal.h>
#include <unistd.h>

struct shm * shmemory;

void clean_all(int mem_id) {
    shmemory = shmat(mem_id, NULL, 0);

    msgctl(shmemory->conf.msgId, IPC_RMID, NULL);
    semctl(shmemory->conf.semId, 0, IPC_RMID);
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
    printf("Error: wait mutex %d\n", errno);
  }
}

void increment_sem(int sem_id, unsigned short sem_num) {
  struct sembuf sops = {sem_num, 1, 0};
  if(semop(sem_id, &sops, 1) == -1) {
    printf("Error:  in increment_sem  %d\n", errno);
  }
}


int create_atoms(char * memid_str, char * a_rand) {
    int cpids;
    char * argq[4] = { ATOMO_NAME };
		argq[1] = memid_str;
		argq[2] = a_rand;
		argq[3] = NULL;
    wait_mutex(shmemory->conf.semId, STATE_SEM);
    shmemory->stat.n_ofatoms++;
    increment_sem(shmemory->conf.semId, STATE_SEM);
		switch(cpids = fork()) {
			case -1:
        kill(shmemory->conf.masterpid, SIGUSR2);
        return cpids;
			case 0:
				if(execve(ATOMO_NAME, argq, NULL) == -1) {
					perror("Error: failed to launch 'atomo'.\n");
					clean_all(shmemory->conf.memconf_id);
          exit(EXIT_FAILURE);
				}
			default:
				break;
		}
  return cpids;
}

int create_process(char * memid_str, char * name) {
  int process_pid;
  char * args[3] = { name };
  args[1] = memid_str;
  switch (process_pid = fork()) {
    case 0:
        if(execve(name, args, NULL) == -1) {
            perror("Error: failed to launch PROCESS.\n");
            clean_all(shmemory->conf.memconf_id);
            exit(EXIT_FAILURE);
        }
        break;
    case -1:
        printf("Error: falied fork to create alimentator process\n");
        exit(EXIT_FAILURE);
    default:
        break;
    }
  
  return process_pid;
}