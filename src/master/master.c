#include "master.h"

struct shmStat* shstatmemory;


void print_stat(int sem_id) {
    printf("SONO QUI\n");
    wait_mutex(sem_id, STATE_SEM);
    printf("SCORIE: %d\n",shstatmemory->TOT_SCORIE);
    printf("SCORIE: %d\n",shstatmemory->TOT_ACTIVATIONS);
    increment_sem(sem_id, STATE_SEM);
}

int main() {
    int status;
    struct shmConf* shconfmemory;
    srand(getpid());
    input_file("../init_file.txt");

    int memconf_id = mem_init(sizeof(&shconfmemory));
    printf("conf %d\n ",memconf_id);
    int memstat_id = mem_init(sizeof(&shstatmemory));
    printf("stat %d\n ",memstat_id);
    shconfmemory = shmat(memconf_id, NULL, 0);
    shstatmemory = shmat(memstat_id, NULL, 0);
    if(shconfmemory == NULL) {
        fprintf(stderr, "Error: failed to attach memory.\n");
        exit(EXIT_FAILURE);
    }

    //start IPC facilities
    shconfmemory->memconf_id = memconf_id;
    shconfmemory->memstat_id = memstat_id;
    shconfmemory->msgId = msg_init();
    shconfmemory->semId = sem_init(2);
    shconfmemory->conf_n_atomi_init = N_ATOMI_INIT;
    shconfmemory->conf_min_atom = MIN_N_ATOMICO;
    shconfmemory->conf_step_attivatore = STEP_ATTIVATORE;
    shconfmemory->conf_n_atom_max = N_ATOM_MAX;
    shconfmemory->conf_n_nuovi_atomi = N_NUOVI_ATOMI;
    shconfmemory->conf_step_alimentatore = STEP_ALIMENTATORE;

    semctl(shconfmemory->semId,0, SETVAL, 1);
    char memid_str[3*sizeof(shconfmemory->memconf_id)+1];
    sprintf(memid_str, "%d", shconfmemory->memconf_id);
 
    //Create activator process
    int activator_process = create_process(memid_str, ACTIVATOR_NAME);

    //Create alimentator process
    int alimentator_process = create_process(memid_str, ALIMENTATOR_NAME);

    //Function to create atoms
    for(int i  = 0; i < shconfmemory->conf_n_atomi_init;  i++) {
        char a_rand[20];
		sprintf(a_rand, "%ld", (rand()%shconfmemory->conf_n_atom_max+1) );
        create_atoms(memid_str, a_rand);
    }

    semctl(shconfmemory->semId,0,SETVAL, 0);
    //provare a mandare un seganale all'alimentatore dicendo di mettersi in wait dei figli
    //quando arriva in questo punto
    // print_stat(shconfmemory->semId);

    while(wait(NULL) > 0) {
		printf("child terminato correttamente.\n");
	}

    clean_all(shconfmemory->memconf_id);
}