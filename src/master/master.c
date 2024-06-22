#include "master.h"


int count_alarm, alimentator_process, activator_process, inhibitor_process;
struct shmStat stat_snap;

void signal_handler(int signum) {
    wait_mutex(shmemory->conf.semId, STATE_SEM);
    master_op();
    memcpy(&stat_snap, &shmemory->stat, sizeof(struct shmStat));

    if(signum == SIGUSR2) {
        // fork failed
        sigset_t mask;
        sigemptyset (&mask);
        sigaddset(&mask, SIGUSR2);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        termination(4);
        return;
    } else {
        if(count_alarm >= SIM_DURATION) {
            termination(1);
            return;
        } else if(shmemory->stat.energy_produced - shmemory->stat.energy_consumed < 0) {
            termination(2);
            return;
        } else if(shmemory->stat.energy_produced - shmemory->stat.energy_consumed > ENERGY_EXPLODE_THRESHOLD) {
            termination(3);
            return;
        }
    }
    
    increment_sem(shmemory->conf.semId, STATE_SEM);
}

int main() {
    srand(getpid());
    input_file("../init_file.txt");

    int mem_id = mem_init();
    shmemory = shmat(mem_id, NULL, 0);
    if(shmemory == NULL) {
        fprintf(stderr, "Error: failed to attach memory.\n");
        exit(EXIT_FAILURE);
    }

    confshm(mem_id);

    semctl(shmemory->conf.semId,0, SETVAL, 1);

    char memid_str[3*sizeof(shmemory->conf.memconf_id)+1];
    sprintf(memid_str, "%d", shmemory->conf.memconf_id);
 
    simulation(memid_str);
  
    alarm(1);
    signal(SIGALRM, signal_handler);
    signal(SIGUSR2, signal_handler);
    semctl(shmemory->conf.semId,0,SETVAL, 0);

    while(1) {
        pause();
    }
}

void simulation(char * memid_str) {
    semctl(shmemory->conf.semId,1, SETVAL, 1);

    activator_process = create_process(memid_str, ACTIVATOR_NAME);
    alimentator_process = create_process(memid_str, ALIMENTATOR_NAME);
    inhibitor_process = create_process(memid_str, INHIBITOR_NAME);
    for(int i  = 0; i < shmemory->conf.conf_n_atomi_init;  i++) {
        char a_rand[20];
		sprintf(a_rand, "%ld", (rand()%shmemory->conf.conf_n_atom_max+1) );
        create_atoms(memid_str, a_rand);
    }
}


void confshm(int mem_id) {
    shmemory->conf.memconf_id = mem_id;
    shmemory->conf.msgId = msg_init();
    shmemory->conf.semId = sem_init(2);
    shmemory->conf.conf_n_atomi_init = N_ATOMI_INIT;
    shmemory->conf.conf_min_atom = MIN_N_ATOMICO;
    shmemory->conf.conf_step_attivatore = STEP_ATTIVATORE;
    shmemory->conf.conf_n_atom_max = N_ATOM_MAX;
    shmemory->conf.conf_n_nuovi_atomi = N_NUOVI_ATOMI;
    shmemory->conf.conf_step_alimentatore = STEP_ALIMENTATORE;
    shmemory->conf.energy_demand = ENERGY_DEMAND;
    shmemory->conf.masterpid = getpid();
    shmemory->conf.conf_energy_thresh = ENERGY_EXPLODE_THRESHOLD;

    shmemory->stat.flags = 0;

}

void master_op() {
    shmemory->stat.energy_consumed+=shmemory->conf.energy_demand;
    printstat(shmemory->conf.semId,&stat_snap);
    count_alarm++;
    alarm(1);
}

void termination(int term) {
    int status;
    switch (term) {
        case 1:
            printf("------------------ TIMEOUT ");
            printf("current duration is %d\nSimulation terimated due to timeout\n",
               count_alarm);
            break;
        case 2:
            printf("------------------ BLACKOUT ");
            printf("current energy is %d\nSimulation terimated due to blackout\n",
               shmemory->stat.energy_produced - shmemory->stat.energy_consumed);
            break;
        case 3:
            printf("------------------ EXPLODE ");
            printf("current energy is %d greater than ENERGY_EXPLODE_THRESHOLD %ld\n",shmemory->stat.energy_produced - shmemory->stat.energy_consumed, ENERGY_EXPLODE_THRESHOLD);
            break;
        case 4:
            printf("------------------ MELTDOWN ------------------ (%d)\n", shmemory->stat.n_ofatoms);
            break;
        default:
            break;
    }

    if(kill(alimentator_process, SIGTERM) == -1) {
        printf("Error to kill alimentator\n");
    }

    if(kill(activator_process, SIGTERM) == -1) {
        printf("Error to kill activator\n");
    }
    
    if(kill(inhibitor_process, SIGTERM) == -1) {
        printf("Error to kill inhibitor\n");
    }
    
    while(wait( &status) != -1) {}
    clean_all(shmemory->conf.memconf_id);
    exit(EXIT_SUCCESS);

}