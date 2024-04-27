#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "../lib/sharedmemory/sharedmemory.h"
#include "../lib/semaphore.h"
#include "../lib/msgqueue.h"

void signal_handler(int signum);