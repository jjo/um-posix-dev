#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>

#include "common.h"
#include "proveedor.h"
#include "cocinero.h"

static int mi_semop(int semid, struct sembuf *sb, int semnum)
{
	int ret;
	if ((ret=semop(semid, sb, semnum)) == -1 && errno != EAGAIN ) {
		perror("semop()");
		_exit(1);
	}
	return ret;
}
void sem_dump(int semid) {
	int i;
	unsigned short array[SEMS_NUM] ;
	union semun semctl_arg;

	semctl_arg.array=array;
	if (semctl(semid, 0, GETALL, semctl_arg ) == -1 )
		{ perror("semctl()"); exit (1); }
	for (i=0;i<SEMS_NUM;i++) printf (" %d ", semctl_arg.array[i]);
	putchar('\n');
}
void proveedor_liberar(semid)
{
	struct sembuf sb[2] = {{ 4, 1, 0}, { 5, 1, 0} };
	mi_semop(semid, sb, 2);
}
void proveedor_esperando(semid)
{
	struct sembuf sb[1] = {{ 4, -1, 0}};
	mi_semop(semid, sb, 1);
}
int proveedor_cocineros_cant(int semid, int cant) {
	struct sembuf sb[1] = {{ 5, -cant, IPC_NOWAIT}};
	int ret=mi_semop(semid, sb, 1);
	//sem_dump(semid);
	return ret;
}
int proveedor_entregar(int semid, int elem[2])
{
	struct sembuf sb[3] = {
		{ elem[0], 1, 0 },
		{ elem[1], 1, 0 },
		{ 4      , 1, 0 }
	};
	return mi_semop(semid, sb, 3);
}
int proveedor_retirar_quizas(int semid, int elem[2])
{
	struct sembuf sb[2] = {
		{ elem[0], -1, IPC_NOWAIT },
		{ elem[1], -1, IPC_NOWAIT },
	};
	return mi_semop(semid, sb, 2);
}
int proveedor_main(int semid)
{
#define SETSIZE 6
	int elementos[SETSIZE][2] = {
		{ 0, 3 },
		{ 1, 2 },
		{ 0, 2 },
		{ 1, 3 },
		{ 0, 1 },
		{ 2, 3 },
	};
	int *elemptr;
	//srand(time(NULL));
	for(;;) {
		elemptr=elementos[rand()%SETSIZE];
		printf("proveedor entregando elem={%d,%d} ...\n", elemptr[0], elemptr[1]);
		proveedor_entregar(semid, elemptr);
		printf("proveedor esperando ...\n");
		proveedor_esperando(semid);
		if (proveedor_retirar_quizas(semid, elemptr)==0)
			printf("proveedor retira no usados\n");
		if (proveedor_cocineros_cant(semid, 6)==0) break;
	}
	return 0;
}
