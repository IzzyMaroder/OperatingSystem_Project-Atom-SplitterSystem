#include "activator.h"

long N_ATOMI_INIT;
tuple *tuplepid;

int main(int argc, char * argv[]) {
    srand(getpid());
    if(argc < 2) {
        fprintf(stderr, "Error: too/many arguments activator.\n");
        clean_all(atoi(argv[1]));
        exit(EXIT_FAILURE);
    }
    if((shmemory = shmat(atoi(argv[1]), NULL, 0)) == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_FAILURE);
    }

    N_ATOMI_INIT = shmemory->conf.conf_n_atomi_init;
    do_scission();
}

void do_scission() {
  struct msg msgqueu;
  int atom, counter = 0, dead = 0;
  tuplepid = malloc(sizeof(tuple) * (shmemory->conf.conf_n_atomi_init));

  while (1) {
    nsleep(shmemory->conf.conf_step_attivatore);
    
    if (msgrcv(shmemory->conf.msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) != -1) {
      if(counter >= (N_ATOMI_INIT)) {
        N_ATOMI_INIT*=2;
        tuple *temp = realloc(tuplepid, N_ATOMI_INIT*sizeof(tuple));
        if(temp == NULL) {
          fprintf(stderr, "Error: cannot reallocate memory.\n");
          clean_all(shmemory->conf.memconf_id);
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
    if ((msgrcv(shmemory->conf.msgId, &msgqueu, sizeof(int), 2, IPC_NOWAIT) != -1)) {
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
        notifyatom(counter);
        free(tuplepid);
        exit(EXIT_SUCCESS);
    }
  }
}

void notifyatom(int counter) {
  for (int j = 0; j < counter; j++) {
    kill(tuplepid[j].pid, SIGTERM);
  }
}