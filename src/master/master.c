#include "master.h"

int main(){
    int status,i;
    srand(getpid());
    input_file("../init_file.txt");

    //allocate initial atoms array
    pid_t * cpids = malloc(sizeof(pid_t) * N_ATOMI_INIT);
    //start IPC facilities
    int mem_id = mem_init();
    int msg_id = msg_init();
    int sem_id = sem_init(2);
    struct shmConf* shconfmemory = shmat(mem_id, NULL, 0);
    if(shconfmemory == NULL) {
        fprintf(stderr, "Error: failed to attach memory.\n");
        exit(EXIT_FAILURE);
    }

    shconfmemory->memId = mem_id;
    shconfmemory->msgId = msg_id;
    shconfmemory->semId = sem_id;
    shconfmemory->conf_n_atomi_init = N_ATOMI_INIT;
    shconfmemory->conf_min_atom = MIN_N_ATOMICO;
    shconfmemory->conf_step_attivatore = STEP_ATTIVATORE;
    shconfmemory->conf_n_atom_max = N_ATOM_MAX;
    shconfmemory->conf_n_nuovi_atomi = N_NUOVI_ATOMI;
    shconfmemory->conf_step_alimentatore = STEP_ALIMENTATORE;
    //check for error
    semctl(sem_id,0, SETVAL, 1);


    char memid_str[3*sizeof(shconfmemory->memId)+1];
    sprintf(memid_str, "%d", mem_id);
 
    //Create activator process
    create_process(memid_str, ACTIVATOR_NAME);

    //Create alimentator process
    create_process(memid_str, ALIMENTATOR_NAME);

    //Function to create atoms
    create_atoms(cpids, memid_str);

	
    semctl(sem_id,0,SETVAL, 0);
    while(wait(&status) != -1) {
		printf("child terminato correttamente con status %d.\n",status);
	}


    clean_all(shconfmemory->memId);
    free(cpids);
}