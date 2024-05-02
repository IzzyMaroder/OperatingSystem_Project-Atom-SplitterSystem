#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>

#define STATE_SEM 1

void clean_all(int mem_id);
void nsleep(long step);
void wait_mutex(int sem_id, unsigned short sem_num);
void increment_sem(int sem_id, unsigned short sem_num);
