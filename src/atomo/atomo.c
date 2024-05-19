#include "atomo.h"

long N_ATOM;

struct msg msgq;
sigset_t mask1;
int main(int argc, char * argv[]) {
    struct sembuf sops;
    sigset_t mask;
    if(argc < 3) {
        fprintf(stderr, "Error: too/many arguments atomo.\n");
        clean_all(atoi(argv[1]));
        exit(EXIT_FAILURE);
    }
    
    if((signal(SIGUSR1, signal_handler) == SIG_ERR) || (signal(SIGTERM, signal_handler) == SIG_ERR)) {
        printf("NON posso.\n");
        exit(EXIT_FAILURE);
    }
    
    if((shmemory = shmat(atoi(argv[1]), NULL, 0)) == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_FAILURE);
    }

    msgq.mtype = 1;
    msgq.pid = getpid();
    if(msgsnd(shmemory->conf.msgId, &msgq, sizeof(int), 0) == -1) {
        printf("Error in msgsnd %d\n", errno);
    }
    N_ATOM = atoi(argv[2]);
    // printf("Il mio num. atomico (ATOMO PADRE): %ld PID: %d\n", N_ATOM, getpid());

    sigemptyset (&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    
    

    while(1) {
        pause();
    }
}

void signal_handler(int signum) {
    if(signum == SIGUSR1) {
        wait_mutex(shmemory->conf.semId, STATE_SEM);
        (N_ATOM <= shmemory->conf.conf_min_atom) ? expiration() : scission();
    } else if(signum == SIGTERM) {
        waitchild();
    }
}

void expiration() {
    // printf("N_ATOM MINORE O UGUALE DI MIN_N_ATOMICO NOTIFICO. PID: %d\n", getpid());
    msgq.mtype = 2;
    if(msgsnd(shmemory->conf.msgId, &msgq, sizeof(int), 0) == -1) {
        printf("Error: in send atomic value.\n");
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_FAILURE);
    }
    shmemory->stat.num_scories++;
    increment_sem(shmemory->conf.semId, STATE_SEM);
    waitchild();
}


void waitchild() {
    exit(EXIT_SUCCESS);
}


void scission() {
    long n_atom_child;
    char n_atom_child_ch[sizeof(int)], mem_str[3*sizeof(int)+1];

    n_atom_child = rand() % ( N_ATOM - 1 )+ 1;
    N_ATOM-=n_atom_child;

    int energy = N_ATOM * n_atom_child - ((N_ATOM > n_atom_child) ? N_ATOM : n_atom_child);
    shmemory->stat.energy_produced+=energy;
    shmemory->stat.num_scissions++;
    increment_sem(shmemory->conf.semId, STATE_SEM);
    // printf("Il mio nuovo num. atomico (ATOMO PADRE) PID: %d N_ATOMICO: %ld\n", getpid(), N_ATOM);
    // printf("Il mio nuovo num. atomico (ATOMO FIGLIO) N_ATOMICO: %ld\n",n_atom_child);
    
    sprintf(n_atom_child_ch,"%ld", n_atom_child);
    sprintf(mem_str, "%d", shmemory->conf.memconf_id);

    create_atoms(mem_str, n_atom_child_ch);
}