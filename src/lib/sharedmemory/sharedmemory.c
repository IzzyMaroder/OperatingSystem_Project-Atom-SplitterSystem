#include "sharedmemory.h"

void print_mem(struct shm * datap) {
    datap->TOT_ACTIVATIONS = datap->TOT_ACTIVATIONS+ datap->ACTIVATIONS_PER_SEC;

    printf("Total activations: %d\n", datap->TOT_ACTIVATIONS);

}