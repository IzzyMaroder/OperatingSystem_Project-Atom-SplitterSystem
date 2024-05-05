#include "../common.h"
#include "../lib/general_lib/general.h"
#include "../lib/msgqueue.h"


extern long ENERGY_DEMAND, N_ATOMI_INIT, N_ATOM_MAX, MIN_N_ATOMICO, STEP_ATTIVATORE, N_NUOVI_ATOMI, STEP_ALIMENTATORE;


void input_file(char * pathname);
int mem_init();
int msg_init();
void simulation(char * memid_str);
int sem_init(int sem_num);
void confshm(int mem_id);
