#include "../common.h"
#include "../lib/msgqueue.h"

typedef struct{
    int pid;
    bool alive;
} tuple;

void do_scission();
void notifyatom(int counter);