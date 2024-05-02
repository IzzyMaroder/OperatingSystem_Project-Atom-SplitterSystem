#define _GNU_SOURCE
#include "../sharedmemory/sharedmemory.h"
#include "general.h"

void clean_all(int mem_id) {
    struct shm *shmemory;
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