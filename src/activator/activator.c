#include "activator.h"

long N_ATOMI_INIT;
int counter = 0;
tuple *tuplepid;
struct msg msgqueu;

void signal_handler() {
  nsleep(99999999);
  while (msgrcv(shmemory->conf.msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) != -1) {
      if (counter >= (N_ATOMI_INIT)) {
        N_ATOMI_INIT *= 2;
        tuple *temp = realloc(tuplepid, N_ATOMI_INIT * sizeof(tuple));
        if (temp == NULL) {
          fprintf(stderr, "Error: cannot reallocate memory.\n");
          clean_all(shmemory->conf.memconf_id);
          exit(EXIT_FAILURE);
        }
        tuplepid = temp;
      }
      tuplepid[counter].pid = msgqueu.pid;
      tuplepid[counter].alive = true;
      counter++;
    }
    while ((msgrcv(shmemory->conf.msgId, &msgqueu, sizeof(int), 2, IPC_NOWAIT) != -1)) {
      for (int i = 0; i < counter; i++) {
        if (tuplepid[i].pid == msgqueu.pid) {
          tuplepid[i].alive = false;
        }
      }
    }
    notifyatom(counter);
    free(tuplepid);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  srand(getpid());
  if (argc < 2) {
    fprintf(stderr, "Error: too/many arguments activator.\n");
    clean_all(atoi(argv[1]));
    exit(EXIT_FAILURE);
  }
  if ((shmemory = shmat(atoi(argv[1]), NULL, 0)) == NULL) {
    fprintf(stderr, "Error: failed to attach memory in atomo.\n");
    clean_all(shmemory->conf.memconf_id);
    exit(EXIT_FAILURE);
  }
  signal(SIGTERM, signal_handler);
  N_ATOMI_INIT = shmemory->conf.conf_n_atomi_init;
  do_scission();
}

void do_scission() {
  int atom, dead = 0;
  tuplepid = malloc(sizeof(tuple) * (shmemory->conf.conf_n_atomi_init));

  while (1) {
    nsleep(shmemory->conf.conf_step_attivatore);

    while (msgrcv(shmemory->conf.msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) !=
           -1) {
      if (counter >= (N_ATOMI_INIT)) {
        N_ATOMI_INIT *= 2;
        tuple *temp = realloc(tuplepid, N_ATOMI_INIT * sizeof(tuple));
        if (temp == NULL) {
          fprintf(stderr, "Error: cannot reallocate memory.\n");
          clean_all(shmemory->conf.memconf_id);
          exit(EXIT_FAILURE);
        }
        tuplepid = temp;
      }
      tuplepid[counter].pid = msgqueu.pid;
      tuplepid[counter].alive = true;
      counter++;
    }
    while ((msgrcv(shmemory->conf.msgId, &msgqueu, sizeof(int), 2, IPC_NOWAIT) != -1)) {
      for (int i = 0; i < counter; i++) {
        if (tuplepid[i].pid == msgqueu.pid) {
          tuplepid[i].alive = false;
          dead++;
        }
      }
    }
    atom = rand() % counter;
    if (tuplepid[atom].alive == true) {
      kill(tuplepid[atom].pid, SIGUSR1);
    }
  }

  if (dead == counter) {
    notifyatom(counter);
  }
}

void notifyatom(int counter) {
  for (int j = 0; j < counter; j++) {
    if (tuplepid[j].alive == true) {
      if (kill(tuplepid[j].pid, SIGTERM) == -1) {
        printf("ERROR: cannot send signal (ACTIVATOR) %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }
  }
}