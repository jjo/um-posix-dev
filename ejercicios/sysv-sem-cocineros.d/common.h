/* Un solo set con 5 semaforos + 1 a modo de contador*/
#define SEMS_NUM     6
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};


