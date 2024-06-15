#include "../common.h"
#include "../lib/general_lib/general.h"
#include "../lib/msgqueue.h"


extern long ENERGY_DEMAND, N_ATOMI_INIT, N_ATOM_MAX, MIN_N_ATOMICO, STEP_ATTIVATORE, N_NUOVI_ATOMI, STEP_ALIMENTATORE, SIM_DURATION, ENERGY_EXPLODE_THRESHOLD;


void input_file(char * pathname);
int mem_init();
int msg_init();
int sem_init(int sem_num);

void printstat(int sem_id,struct shmStat*);
void simulation(char * memid_str);
void confshm(int mem_id);
void master_op();
void check_for_termination();
void termination(int term);