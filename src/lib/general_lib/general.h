#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>

void clean_all(int mem_id);
void nsleep(long step);