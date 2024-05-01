#define _GNU_SOURCE
#include "activator.h"

long N_ATOMI_INIT, STEP_ATTIVATORE;
struct shm *shmemory;
struct msg msgqueu;
tuple *tuplepid;
struct timespec reqtime, retime;

int main(int argc, char * argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments activator.\n");
        clean_all(atoi(argv[1]));
        exit(EXIT_FAILURE);
    }
    srand(getpid());
    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        clean_all(shmemory->memId);
        exit(EXIT_FAILURE);
    }
    N_ATOMI_INIT = shmemory->conf_n_atomi_init;
    STEP_ATTIVATORE = shmemory->conf_step_attivatore;
    //popolare array
    do_scission();
}

void do_scission() {
  int msgId, atom, counter = 0, dead = 0;
  msgId = shmemory->msgId;
  reqtime.tv_sec = 1;
  reqtime.tv_nsec = STEP_ATTIVATORE;
  tuplepid = malloc(sizeof(tuple) * (shmemory->conf_n_atomi_init));

  while (1) {
    // nanosleep(&reqtime , &retime);
    sleep(1);
    if (msgrcv(msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) != -1) {
      if(counter >= (N_ATOMI_INIT)) {
        N_ATOMI_INIT*=2;
        tuple *temp = realloc(tuplepid, N_ATOMI_INIT*sizeof(tuple));
        if(temp == NULL) {
          fprintf(stderr, "Error: cannot reallocate memory.\n");
          clean_all(shmemory->memId);
          exit(EXIT_FAILURE);
        }
        tuplepid = temp;
      }
      printf("PID: %d, MTYPE: %ld\n", msgqueu.pid, msgqueu.mtype);
      tuplepid[counter].pid = msgqueu.pid;
      tuplepid[counter].alive = true;
      counter++;
      printf("COUNTER: %d\n", counter);
    }
    reqtime = retime;
    if ((msgrcv(msgId, &msgqueu, sizeof(int), 2, IPC_NOWAIT) != -1)) {
      for (int i = 0; i < counter; i++) {
        if (tuplepid[i].pid == msgqueu.pid) {
          tuplepid[i].alive = false;
          dead++;
          printf("DEAD: %d\n", dead);
        }
      }
    } else {
      atom = rand() % counter;
      if (tuplepid[atom].alive == true) {
        kill(tuplepid[atom].pid, SIGUSR1);
        printf("PID: %d ALIVE: %d INVIO SEGNALE.\n", tuplepid[atom].pid, tuplepid[atom].alive);
      }
    }

    if (dead == counter) {
        terminate(counter);
        clean_all(shmemory->memId);
        free(tuplepid);
        exit(EXIT_SUCCESS);
    }
  }
}

void terminate(int counter) {
  for (int j = 0; j < counter; j++) {
    kill(tuplepid[j].pid, SIGTERM);
  }
}