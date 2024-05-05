#include "master.h"

int count_alarm, alimentator_process, activator_process;

void signal_handler() {
    wait_mutex(shmemory->conf.semId, STATE_SEM);
    master_op();
    increment_sem(shmemory->conf.semId, STATE_SEM);
    termination();
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
  
    semctl(shmemory->conf.semId,0,SETVAL, 0);
    //provare a mandare un seganale all'alimentatore dicendo di mettersi in wait dei figli
    //quando arriva in questo punto

    signal(SIGALRM, signal_handler);
    alarm(1);

    while(1) {
        pause();
    }
    
}

void simulation(char * memid_str) {
    semctl(shmemory->conf.semId,1, SETVAL, 1);

    activator_process = create_process(memid_str, ACTIVATOR_NAME);
    alimentator_process = create_process(memid_str, ALIMENTATOR_NAME);

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
}

void master_op() {
    printstat(shmemory->conf.semId);
    count_alarm++;
    shmemory->stat.energy_consumed+=shmemory->conf.energy_demand;
    count_alarm++;
    alarm(1);
}

void termination() {
    if(count_alarm >= SIM_DURATION) {
        waitprocess(alimentator_process);
        waitprocess(activator_process);
        printf("------------------ TIMEOUT ");
        printstat(shmemory->conf.semId);
        clean_all(shmemory->conf.memconf_id);
        exit(EXIT_SUCCESS);

    } 
    // else if(shmemory->stat.energy_produced - shmemory->stat.energy_consumed < 0) {
    //     waitprocess(alimentator_process);
    //     waitprocess(activator_process);
    //     printf("------------------ BLACKOUT ");
    //     printstat(shmemory->conf.semId);
    //     clean_all(shmemory->conf.memconf_id);
    //     exit(EXIT_SUCCESS);
    // }
    // } else if(shmemory->stat.energy_produced - shmemory->stat.energy_consumed < 0) {
    //     waitprocess(alimentator_process);
    //     printf("------------------ EXPLODE ");
    //     printstat(shmemory->conf.semId);
    //     clean_all(shmemory->conf.memconf_id);
    //     exit(EXIT_SUCCESS);
    // }
}