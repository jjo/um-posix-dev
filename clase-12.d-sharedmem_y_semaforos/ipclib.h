/* ipclib.c */
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
void *shm_create(key_t key, size_t size, int getflags, int atflags);
int sem_create(key_t key, size_t n_sems, int getflags);
int sem_up_flg(int sem_id, int sem_num, int delta, int flg);
static __inline__ int sem_down_flg(int id, int num, int delta, int flg)
{
	return sem_up_flg(id, num, -delta, flg);
}
static __inline__ int sem_up(int id, int num)
{
	return sem_up_flg(id, num, 1, 0);
}
static __inline__ int sem_down(int id, int num)
{
	return sem_up_flg(id, num, -1, 0);
}

