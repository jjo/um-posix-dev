#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/sem.h>

#include "cocinero.h"
#include "proveedor.h"
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

/* Un solo set con 5 semaforos */
#define SEMS_NUM     5
int mi_sem_open(void)
{
	int semid;
	union semun semctl_arg;
	unsigned short array[SEMS_NUM] = { 0, 0, 0, 0, 0};

	semctl_arg.array=array;
	
	if((semid=semget(ftok(".",0), SEMS_NUM, IPC_CREAT|0666)) == -1)
		{ perror("semget()"); exit (1); }

	if (semctl(semid, 0, SETALL, semctl_arg ) == -1 )
		{ perror("semctl()"); exit (1); }
	return semid;
}
int main(void) 
{
	int i;
	int semid;
	semid=mi_sem_open();
	for (i=0;i<COCINEROS_NUM;i++) {
		if (cocinero_crea(i,semid)<0)
			return 2;
	}
	proveedor_main(semid);	
	while(waitpid(0, NULL, 0)>=0);
	return 0;
}
