#include "master.h"

int count_alarm;

// void print_stat(int sem_id) {
//     wait_mutex(sem_id, STATE_SEM);
//     printf("SCORIE: %d\n",shstatmemory->TOT_SCORIE);
//     // printf("SCORIE: %d\n",shstatmemory->TOT_ACTIVATIONS);
//     increment_sem(sem_id, STATE_SEM);
//     alarm(1);
// }

// void signal_handler() {
//     count_alarm++;
//     print_stat(shmemory->semId);
//     if(count_alarm  >= 4) {
        // while(wait(NULL) > 0) {
		//     printf("child terminato correttamente.\n");
	    // }
//         exit(EXIT_SUCCESS);
//     }
// }



int main() {
    int status;
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
    semctl(shmemory->conf.semId,1, SETVAL, 1);
    char memid_str[3*sizeof(shmemory->conf.memconf_id)+1];
    sprintf(memid_str, "%d", shmemory->conf.memconf_id);
 
    simulation(memid_str);
  
    semctl(shmemory->conf.semId,0,SETVAL, 0);
    //provare a mandare un seganale all'alimentatore dicendo di mettersi in wait dei figli
    //quando arriva in questo punto


    // signal(SIGALRM, signal_handler);
    // alarm(1);
    // print_stat(shmemory->conf.semId);
    // while(1) {
    //     pause();
    // }
    
    while(wait(NULL) > 0) {
		printf("child terminato correttamente.\n");
	}

    clean_all(shmemory->conf.memconf_id);
}

void simulation(char * memid_str) {
    int activator_process = create_process(memid_str, ACTIVATOR_NAME);
    int alimentator_process = create_process(memid_str, ALIMENTATOR_NAME);

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
}