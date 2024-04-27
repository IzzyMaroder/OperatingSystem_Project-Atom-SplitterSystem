#include "master.h"

#define ATOMO_NAME "./atomo"
#define ACTIVATOR_NAME "./activator"

//Qui uso un long per tutto dato che i STEP devono per forza stare in un long

int main(int argc,char *arvg[]){
    int status,i;
    srand(getpid());
    input_file("../init_file.txt");

    //allocate initial atoms array
    pid_t * cpids = malloc(sizeof(pid_t) * N_ATOMI_INIT);
    //start IPC facilities
    int mem_id = mem_init();
    int msg_id = msg_init();
    int sem_id = sem_init(1);
    char memid_str[3*sizeof(sem_id)+1];
    struct shm* shared = shmat(mem_id, NULL, 0);
    if(shared == NULL) {
        fprintf(stderr, "Error: failed to attach memory.\n");
        exit(EXIT_FAILURE);
    }

    shared->memId = mem_id;
    shared->msgId = msg_id;
    shared->semId = sem_id;
    shared->min_atom = MIN_N_ATOMICO;
    //check for error
    semctl(sem_id,0, SETVAL, 1);
    //create initial processes
    int activator_pid;
    char * args[3] = { ACTIVATOR_NAME };
    sprintf(memid_str, "%d", mem_id);
    args[1] = memid_str;

    switch (activator_pid = fork()) {
        //child process
        case 0:
            if(execve(ACTIVATOR_NAME, args, NULL) == -1) {
                perror("Error: failed to launch 'activator'.\n");
                exit(EXIT_FAILURE);
            }
            break;
        //error
        case -1:
            printf("Error: falied fork to create activator process");
            exit(EXIT_FAILURE);
        default:
            break;
    }
    int n_atom_rand;
    char atom_rand[20];

	for(i = 0; i < N_ATOMI_INIT;  i++) {
		n_atom_rand = rand()%N_ATOM_MAX+1;
		sprintf(atom_rand, "%d", n_atom_rand);
        char * argq[4] = { ATOMO_NAME };
		argq[1] = memid_str;
		argq[2] = atom_rand;
		argq[3] = NULL;
		switch(cpids[i] = fork()) {
			case -1:
				fprintf(stderr,"Error: failed to fork.\n");
				exit(EXIT_FAILURE);
			case 0:
				if(execve(ATOMO_NAME, argq, NULL) == -1) {
					perror("Error: failed to launch 'atomo'.\n");
					exit(EXIT_FAILURE);
				}
			default:
				break;
		}
	}
    semctl(sem_id,0,SETVAL, 0);
    while(wait(&status) != -1) {
		printf("child terminato correttamente.\n");
	}

    //creare funzione che cancella tutta la merda.
    shmdt(shared);
    shmctl( mem_id , IPC_RMID , NULL );
    semctl(sem_id, 0, IPC_RMID);
    free(cpids);

}