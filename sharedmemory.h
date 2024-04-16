struct shm {
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
    int memId;
};

struct shm* mem_init();
void print_mem(struct shm * datap);
void clean_mem(struct shm * datap);