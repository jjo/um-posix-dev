#include <stdio.h>
#include <unistd.h>
#include "cocinero.h"
#include "proveedor.h"

#include <sys/sem.h>
struct cocinero {
	char *name;	
	struct sembuf sembuf[2];
};
struct cocinero cocineros[COCINEROS_NUM] = {
	{ "C1", { {0}, {1} } },
	{ "C2", { {0}, {2} } },
	{ "C3", { {0}, {3} } },
	{ "C4", { {1}, {2} } },
	{ "C5", { {1}, {3} } },
	{ "C6", { {2}, {3} } },
};

int cocinero_main(int num, int semid)
{
	struct cocinero *cocinero=&cocineros[num]; /* = cocineros+num */
	cocinero->sembuf[0].sem_op=-1;
	cocinero->sembuf[0].sem_flg=0;
	cocinero->sembuf[1].sem_op=-1;
	cocinero->sembuf[1].sem_flg=0;
	semop(semid, cocinero->sembuf, 2);
	printf("  cocinero \"%s\" laburando ... ", cocinero->name);
	usleep(300000);
	puts("listo.");
	return 0;
}
int cocinero_crea(int num, int semid) 
{
	int pid;
	if ((pid=fork())==0) {
		int ret;
		ret=cocinero_main(num,semid);
		proveedor_liberar(semid);
		_exit(ret);
	}
	return pid;
}
