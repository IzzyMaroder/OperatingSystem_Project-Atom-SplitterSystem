

#define STATE_SEM 1
#define ATOMO_NAME "./atomo"
#define ACTIVATOR_NAME "./activator"
#define INHIBITOR_NAME "./inhibitor"
#define ALIMENTATOR_NAME "./alimentator"

void clean_all(int mem_id);
void nsleep(long step);
void wait_mutex(int sem_id, unsigned short sem_num);
void increment_sem(int sem_id, unsigned short sem_num);

int create_atoms(char * memid_str, char * rand);
int create_process(char * memid_str, char * name);