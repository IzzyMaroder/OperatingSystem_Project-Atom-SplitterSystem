#include "atomo.h"
#include <stdio.h>

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
    
    if((signal(SIGUSR1, signal_handler) == SIG_ERR) || signal(SIGTERM, signal_handler) == SIG_ERR) {
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
        exit(0);
    }
    N_ATOM = atoi(argv[2]);

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
        shmemory->stat.num_activations++;
        increment_sem(shmemory->conf.semId, STATE_SEM);
        (N_ATOM <= shmemory->conf.conf_min_atom) ? expiration() : scission();
    } else if(signum == SIGTERM) {
        waitchild();
    }
}

void expiration() {
    msgq.mtype = 2;
    if(msgsnd(shmemory->conf.msgId, &msgq, sizeof(int), 0) == -1) {
        printf("Error: in send atomic value.\n");
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_FAILURE);
    }
    wait_mutex(shmemory->conf.semId, STATE_SEM);
    shmemory->stat.num_scories++;
    increment_sem(shmemory->conf.semId, STATE_SEM);
    waitchild();
}


void waitchild() {
    exit(EXIT_SUCCESS);
}


void scission() {
    if(shmemory->stat.flags == 0) {
        long n_atom_child;
        char n_atom_child_ch[2*sizeof(long)+1], mem_str[3*sizeof(int)+1];
        double temp_absorbed;

        n_atom_child = rand() % ( N_ATOM - 1 )+ 1;
        N_ATOM-=n_atom_child;

        int energy = N_ATOM * n_atom_child - ((N_ATOM > n_atom_child) ? N_ATOM : n_atom_child);
        
        temp_absorbed = (energy) * (shmemory->stat.energy_to_remove);
        printf("TEMP ABSO %f TEMP ENERGY%d\n", temp_absorbed, energy);

        wait_mutex(shmemory->conf.semId, STATE_SEM);
        
        shmemory->stat.energy_absorbed+=temp_absorbed;
        energy-=temp_absorbed;
        printf("ABSORBEDDDD %f\n",shmemory->stat.energy_absorbed);
        shmemory->stat.energy_produced+=energy;
        shmemory->stat.num_scissions++;
        increment_sem(shmemory->conf.semId, STATE_SEM);
        
        sprintf(n_atom_child_ch,"%ld", n_atom_child);
        sprintf(mem_str, "%d", shmemory->conf.memconf_id);

        if(create_atoms(mem_str, n_atom_child_ch) == -1){
            pause();
        }
    }
}