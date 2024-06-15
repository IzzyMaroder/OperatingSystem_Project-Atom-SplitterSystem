#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shmConf {
    int memconf_id;
    int memstat_id;
    int semId;
    int msgId;
    int masterpid;

    long conf_min_atom, energy_demand;
    long conf_n_atomi_init;
    long conf_step_attivatore;
    long conf_n_atom_max;
    long conf_n_nuovi_atomi;
    long conf_step_alimentatore;
};

struct shmStat {
    int energy_produced, num_scories, num_scissions, 
    num_activations, energy_consumed, energy_absorbed;
};

struct shm {
    struct shmStat stat;
    struct shmConf conf;
};
