#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define STATE_SEM 1
#define ATOMO_NAME "./atomo"
#define ACTIVATOR_NAME "./activator"
#define ALIMENTATOR_NAME "./alimentator"

void clean_all(int mem_id);
void nsleep(long step);
void wait_mutex(int sem_id, unsigned short sem_num);
void increment_sem(int sem_id, unsigned short sem_num);

void create_atoms(char * memid_str, char * rand);
void create_process(char * memid_str, char * name);