#include "master.h"

int main() {

    int status;
    srand(getpid());
    input_file("../init_file.txt");

    int mem_id = mem_init();
    struct shmConf* shconfmemory = shmat(mem_id, NULL, 0);
    if(shconfmemory == NULL) {
        fprintf(stderr, "Error: failed to attach memory.\n");
        exit(EXIT_FAILURE);
    }

    //start IPC facilities
    shconfmemory->memId = mem_id;
    shconfmemory->msgId = msg_init();
    shconfmemory->semId = sem_init(2);
    shconfmemory->conf_n_atomi_init = N_ATOMI_INIT;
    shconfmemory->conf_min_atom = MIN_N_ATOMICO;
    shconfmemory->conf_step_attivatore = STEP_ATTIVATORE;
    shconfmemory->conf_n_atom_max = N_ATOM_MAX;
    shconfmemory->conf_n_nuovi_atomi = N_NUOVI_ATOMI;
    shconfmemory->conf_step_alimentatore = STEP_ALIMENTATORE;

    semctl(shconfmemory->semId,0, SETVAL, 1);
    char memid_str[3*sizeof(shconfmemory->memId)+1];
    sprintf(memid_str, "%d", shconfmemory->memId);
 
    //Create activator process
    create_process(memid_str, ACTIVATOR_NAME);

    //Create alimentator process
    create_process(memid_str, ALIMENTATOR_NAME);

    //Function to create atoms
    for(int i  = 0; i < shconfmemory->conf_n_atomi_init;  i++) {
        char a_rand[20];
		sprintf(a_rand, "%ld", (rand()%shconfmemory->conf_n_atom_max+1) );
        create_atoms(memid_str, a_rand);
    }

    semctl(shconfmemory->semId,0,SETVAL, 0);
    while(wait(&status) != -1) {
		printf("child terminato correttamente con status %d.\n",status);
	}

    clean_all(shconfmemory->memId);
}