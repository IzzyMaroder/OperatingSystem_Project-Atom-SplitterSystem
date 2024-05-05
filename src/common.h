#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <stdbool.h>
#include "./lib/sharedmemory/sharedmemory.h"
#include "./lib/general_lib/general.h"

extern struct shm *shmemory;