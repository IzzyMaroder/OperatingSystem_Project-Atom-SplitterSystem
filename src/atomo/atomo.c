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
        fprintf(stderr, "Error: too/many arguments atomo.\n");
        clean_all(atoi(argv[1]));
        exit(EXIT_FAILURE);
    }
    
    if(signal(SIGUSR1, signal_handler) == SIG_ERR) {
        printf("NON posso.\n");
    }
    if(signal(SIGTERM, signal_handler) == SIG_ERR) {
        printf("NON posso.\n");
    }
    sharedmem = shmat(atoi(argv[1]), NULL, 0);
    if(sharedmem  == NULL) {
        fprintf(stderr, "Error: failed to attach memory in atomo.\n");
        clean_all(sharedmem->memId);
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
    while(1) {
        pause();
    }
}

void signal_handler(int signum) {
    printf("Ho ricevuto il segnale SIGNUM: %d, PID: %d\n", signum, getpid());
    if(signum == 15) {
        printf("receives sigterm\n");
        termination();
    }
    if(signum == 10) {
        scission();
    }
}

void termination() {
    int child_status;
    while (wait(&child_status) != -1) {
        printf("atom waited a child atom\n");
    }
    exit(EXIT_SUCCESS);
}


/* questa funzione avvia il processo di scissione, calcolando il
nuovo valore del numero atomico (N_ATOM). All'execve passo tre valori
il valore del numero atomico per il figlio scisso e il valore della mem
condivisa */
void scission() {
    int child_atom, child_wait;
    long n_atom_child;
    char n_atom_child_ch[sizeof(int)];
    char mem_str[3*sizeof(int)+1];


    if(N_ATOM <= MIN_N_ATOMICO) {
        printf("N_ATOM MINORE O UGUALE DI MIN_N_ATOMICO NOTIFICO. PID: %d\n", getpid());
        msgq.mtype = 2;
        if(msgsnd(sharedmem->msgId, &msgq, sizeof(int), 0) == -1) {
            printf("Error: in send atomic value.\n");
            clean_all(sharedmem->memId);
            exit(EXIT_FAILURE);
        }
        termination();
    }

    n_atom_child = rand() % ( N_ATOM - 1 )+ 1;
    N_ATOM-=n_atom_child;

    printf("Il mio nuovo num. atomico (ATOMO PADRE) PID: %d N_ATOMICO: %ld\n", getpid(), N_ATOM);
    printf("Il mio nuovo num. atomico (ATOMO FIGLIO) N_ATOMICO: %ld\n",n_atom_child);
    
    char * argp[4] = { ATOMO_NAME };
    sprintf(n_atom_child_ch,"%ld", n_atom_child);
    sprintf(mem_str, "%d", sharedmem->memId);
    argp[1] = mem_str;
    argp[2] = n_atom_child_ch;
    argp[3] = NULL;
    switch (child_atom = fork()) {
        case -1:
            fprintf(stderr,"Error: failed to fork.\n");
            clean_all(sharedmem->memId);
            exit(EXIT_FAILURE);
        case 0:
            if(execve(ATOMO_NAME, argp, NULL) == -1) {
                perror("(ATOMO) Error: failed to launch 'atomo'.\n");
                clean_all(sharedmem->memId);
                exit(EXIT_FAILURE);
            }
        default:
            break;
    }
}



// void aa {
//     struct sembuf sbuf =  {
//         sem_num = 1; //ID sem. Indicizzato ad array
//         sem_op = -1 //finchè non raggiunge questo o magg.
//         sem_flg = 0;
//     };
//     semop(sem_id, &sbuf, 1);
//     mem;
//     sbuf.sem_op = 1;
//     semop();
// }