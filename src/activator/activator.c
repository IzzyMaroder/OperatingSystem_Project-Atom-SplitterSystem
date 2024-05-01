#include <sys/wait.h>
#define _GNU_SOURCE
#include "activator.h"

long N_ATOMI_INIT, STEP_ATTIVATORE;
struct shm *shmemory;
struct  msg msgqueu;
struct timespec reqtime, retime;

int main(int argc, char * argv[]) {
    srand(getpid());
    shmemory = shmat(atoi(argv[1]), NULL, 0);
    if(shmemory  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    N_ATOMI_INIT = shmemory->conf_n_atomi_init;
    STEP_ATTIVATORE = shmemory->conf_step_attivatore;
    //popolare array
    do_scission();
}

void do_scission() {
    int msgId, atom, counter = 0, dead = 0;
    msgId = shmemory->msgId;
    reqtime.tv_sec = 1;
    reqtime.tv_nsec = STEP_ATTIVATORE;
    tuple *tuplepid = malloc(sizeof(tuple) * (N_ATOMI_INIT * 4));
    // fare allocazione mem dinamica
    // sleep(1);

    while(1) {
        // nanosleep(&reqtime , &retime);
        sleep(1);
        if(msgrcv(msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) != -1) {
            printf("PID: %d, MTYPE: %ld\n", msgqueu.pid, msgqueu.mtype);
            tuplepid[counter].pid = msgqueu.pid;
            tuplepid[counter].alive = true;
            counter++;
            printf("COUNTER: %d\n", counter);
        }

        // printf("ATOM: %d\n", atom);

        reqtime = retime;
        if((msgrcv(msgId, &msgqueu, sizeof(int), 2, IPC_NOWAIT) != -1)) {
            for(int i = 0; i < counter; i++) {
                if(tuplepid[i].pid == msgqueu.pid) {
                    tuplepid[i].alive = false;
                    dead++;
                    printf("DEAD: %d\n", dead);
                }
            }
        }else {
            atom = rand() % counter;
            // printf("ATOM: %d\n", atom);
            // printf("ALIVE %d\n",tuplepid[atom].alive);
            if(tuplepid[atom].alive == true ) {
                kill(tuplepid[atom].pid, SIGUSR1);
                printf("PID: %d ALIVE: %d INVIO SEGNALE.\n",tuplepid[atom].pid, tuplepid[atom].alive);
            }
        }

        if (dead == counter) {
            printf("ATTIVATORE: HO FINITO.\n");
            msgctl (msgId, IPC_RMID , NULL);
            free(tuplepid);
            exit(EXIT_SUCCESS);
        }
    }
}

























    // while(1) {
    //     // printf("sono qui\n");
    //     // do {
    //     //     nanosleep(&reqtime , &retime);
    //     //     reqtime = retime;
    //     // } while(errno == EINTR);
    //     // while ((msgrcv(msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) != -1)) {
            // printf("PID: %d, MTYPE: %ld\n", msgqueu.pid, msgqueu.mtype);
            // tuplepid[counter].pid = msgqueu.pid;
            // tuplepid[counter].alive = true;
            // counter++;
            // printf("COUNTER: %d\n", counter);
    //     // }
    //     // printf("esco\n");
    //     do {
    //         // atom = rand() % counter;
    //         if((msgrcv(msgId, &msgqueu, sizeof(int), 1, IPC_NOWAIT) != -1)) {
    //             printf("PID_: %d, MTYPE_: %ld\n", msgqueu.pid, msgqueu.mtype);
    //             tuplepid[counter].pid = msgqueu.pid;
    //             tuplepid[counter].alive = true;
    //             counter++;
    //             printf("COUNTER_: %d\n", counter);
    //         }
    //         atom = rand() % counter;
    //         sleep(1);
    //         printf("ATOM: %d\n", atom);
    //         printf("ATOM ALIVE: %d\n", tuplepid[atom].alive);
    //     } while (tuplepid[atom].alive == 0); //false = 0 true = 1
        
    //     printf("ESCO\n");
    //     // printf("TUPLEPID: %d\n",tuplepid[atom].pid);
    //     while ((msgrcv(msgId, &msgqueu, sizeof(int), 2, IPC_NOWAIT) != -1)) {
    //         for(int i = 0; i < counter; i++) {
    //             if(tuplepid[i].pid == msgqueu.pid) {
    //                 tuplepid[i].alive = false;
    //             }
    //         }
    //         dead++;
    //         printf("DEAD: %d\n", dead);
    //     }
        
        // if (dead == counter) {
        //     exit(EXIT_SUCCESS);
        // }
    //     do {
    //         nanosleep(&reqtime , &retime);
    //         reqtime = retime;
    //     } while(errno == EINTR);
    //     printf("DOVREI %d\n", tuplepid[atom].pid);
    //     // sleep(1);
    //     kill(tuplepid[atom].pid, SIGUSR1);
    // }