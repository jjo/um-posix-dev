#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#include "proveedor.h"
#include "cocinero.h"

static int mi_semop(int semid, struct sembuf *sb, int semnum)
{
	int ret;
	if ((ret=semop(semid, sb, semnum)) == -1 ) {
		perror("semop()");
		_exit(1);
	}
	return ret;
}
void proveedor_liberar(semid)
{
	struct sembuf sb[1] = {{ 4, 1, 0}};
	mi_semop(semid, sb, 1);
}
void proveedor_esperando(semid)
{
	struct sembuf sb[1] = {{ 4, -1, 0}};
	mi_semop(semid, sb, 1);
}
int proveedor_entregar(int semid, int elem[2])
{
	struct sembuf sb[2] = {
		{ elem[0], 1, 0 },
		{ elem[1], 1, 0 }
	};
	return mi_semop(semid, sb, 2);
}
int proveedor_main(int semid)
{
	int i;
	int elementos[COCINEROS_NUM][2] = {
		{ 0, 3 },
		{ 1, 2 },
		{ 0, 2 },
		{ 1, 3 },
		{ 0, 1 },
		{ 2, 3 },
	};
	for(i=0;i<COCINEROS_NUM;i++) {
		printf("entregando elem={%d,%d} ...\n", elementos[i][0], elementos[i][1]);
		proveedor_entregar(semid, elementos[i]);
		proveedor_esperando(semid);
	}
	return 0;
}
