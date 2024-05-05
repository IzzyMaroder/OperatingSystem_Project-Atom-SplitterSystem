#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shmConf {
    int memconf_id;
    int memstat_id;
    int semId;
    int msgId;

    long conf_min_atom;
    long conf_n_atomi_init;
    long conf_step_attivatore;
    long conf_n_atom_max;
    long conf_n_nuovi_atomi;
    long conf_step_alimentatore;
};

struct shmStat {
    int CURRENT_ENERGY;

    int TOT_ACTIVATIONS;
    int ACTIVATIONS_PER_SEC;

    int TOT_SCISSIONS;
    int SCISSIONS_PER_SEC;

    int TOT_PRODUCED_ENERGY;
    int PRODUCED_ENERGY_PER_SEC;

    int TOT_CONSUMED_ENERGY;
    int CONSUMED_ENERGY_PER_SEC;


    int TOT_SCORIE;
    int SCORIE_PER_SEC;
    int ABSORBED_ENERGY;

};

struct shm {
    struct shmStat stat;
    struct shmConf conf;
};

// struct shm* mem_init();
