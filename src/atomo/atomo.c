#include "atomo.h"
#define ATOMO_NAME "./atomo"

long N_ATOM, MIN_N_ATOMICO;

struct shm * sharedmem;
struct msg msgq;
struct sigaction sa;

int main(int argc, char * argv[]) {
    struct sembuf sops;
    int child_status = 1;
    sigset_t mask;
    if(argc < 3) {
        fprintf(stderr, "Error: too/many arguments.\n");
        exit(EXIT_FAILURE);
    }
    
    if(signal(SIGUSR1, signal_handler) == SIG_ERR) {
        printf("NON posso.\n");
    }
    sharedmem = shmat(atoi(argv[1]), NULL, 0);
    if(sharedmem  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        exit(EXIT_FAILURE);
    }
    int sem_id = sharedmem->semId;
    int msgId = sharedmem->msgId;
    msgq.mtype = 1;
    msgq.pid = getpid();
    msgsnd(msgId, &msgq, sizeof(int), 0);
    N_ATOM = atoi(argv[2]);
    printf("Il mio num. atomico (ATOMO PADRE): %ld PID: %d\n", N_ATOM, getpid());

    MIN_N_ATOMICO = sharedmem->conf_min_atom;
    sigemptyset (&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    pause();
    exit(EXIT_SUCCESS);
}

void signal_handler(int signum) {
    printf("Ho ricevuto il segnale, %d\n", getpid());
    scission();
}

/* questa funzione avvia il processo di scissione, calcolando il
nuovo valore del numero atomico (N_ATOM). All'execve passo tre valori
il valore del numero atomico per il figlio scisso e il valore della mem
condivisa */

void scission() {
    int child_atom, child_status, child_wait;
    long n_atom_child;
    char n_atom_child_ch[sizeof(int)];
    char mem_str[3*sizeof(int)+1];
    if(N_ATOM % 2 == 0) {
        N_ATOM = N_ATOM/2;
        n_atom_child = N_ATOM;
        printf("Il mio nuovo num. atomico (ATOMO PADRE): %ld\n", N_ATOM);
        printf("Il mio nuovo num. atomico (ATOMO FIGLIO): %ld\n", n_atom_child);
    }else {
        if(N_ATOM <= MIN_N_ATOMICO) {
            printf("N_ATOM MINORE_ O UGUALE DI MIN_N_ATOMICO NOTIFICO. PID: %d\n", getpid());
            msgq.mtype = 2;
            if(msgsnd(sharedmem->msgId, &msgq, sizeof(int), 0) == -1) {
                printf("Error: in send atomic value.\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }else {
            N_ATOM = N_ATOM/2;
            n_atom_child = N_ATOM+1;
            printf("Il mio nuovo num. atomico (ATOMO PADRE): %ld\n", N_ATOM);
            printf("Il mio nuovo num. atomico (ATOMO FIGLIO): %ld\n", n_atom_child);
        }
    }
    char * argp[4] = { ATOMO_NAME };
    sprintf(n_atom_child_ch,"%ld", n_atom_child);
    sprintf(mem_str, "%d", sharedmem->memId);
    argp[1] = mem_str;
    argp[2] = n_atom_child_ch;
    argp[3] = NULL;
    switch (child_atom = fork()) {
        case -1:
            fprintf(stderr,"Error: failed to fork.\n");
            exit(EXIT_FAILURE);
        case 0:
            if(execve(ATOMO_NAME, argp, NULL) == -1) {
                perror("(ATOMO) Error: failed to launch 'atomo'.\n");
                exit(EXIT_FAILURE);
            }
        default:
            break;
    }
    
    // while(wait(&child_status) != -1) {
    //     printf("Termino l'atomo figlio.\n");

    // }

    if(N_ATOM <= MIN_N_ATOMICO) {
        printf("N_ATOM MINORE O UGUALE DI MIN_N_ATOMICO NOTIFICO. PID: %d\n", getpid());
        msgq.mtype = 2;
        if(msgsnd(sharedmem->msgId, &msgq, sizeof(int), 0) == -1) {
            printf("Error: in send atomic value.\n");
            exit(EXIT_FAILURE);
        }
    }
    
}