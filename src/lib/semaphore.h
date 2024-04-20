#ifdef PROTECT_SHM
#define NUM_SEMS 2
#define LOCK					\
	sops.sem_num = 1;			\
	sops.sem_op = -1;			\
	sops.sem_flg = 0;			\
	semop(s_id, &sops, 1);
#define UNLOCK					\
	sops.sem_num = 1;			\
	sops.sem_op = 1;			\
	sops.sem_flg = 0;			\
	semop(s_id, &sops, 1);
#else
#define NUM_SEMS 1
#define LOCK
#define UNLOCK
#endif