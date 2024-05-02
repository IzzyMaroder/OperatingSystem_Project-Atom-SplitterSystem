#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include "../lib/sharedmemory/sharedmemory.h"
#include "../lib/general_lib/general.h"
#include "../lib/semaphore.h"
#include "../lib/msgqueue.h"

typedef struct{
    int pid;
    bool alive;
} tuple;

void do_scission();
void terminate(int counter);