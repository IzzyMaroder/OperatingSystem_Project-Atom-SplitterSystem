#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <errno.h>
#include "../lib/general_lib/general.h"
#include "../lib/msgqueue.h"
#include "../lib/sharedmemory/sharedmemory.h"

void signal_handler(int signum);
void scission();
void termination();