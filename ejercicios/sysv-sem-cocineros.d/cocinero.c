#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "cocinero.h"
#include "proveedor.h"

#include <sys/sem.h>
struct cocinero {
	char *name;	
	struct sembuf sembuf[3];
};
struct cocinero cocineros[COCINEROS_NUM] = {
	{ "C{0,1}", { {0}, {1} } },
	{ "C{0,2}", { {0}, {2} } },
	{ "C{0,3}", { {0}, {3} } },
	{ "C{1,2}", { {1}, {2} } },
	{ "C{1,3}", { {1}, {3} } },
	{ "C{2,3}", { {2}, {3} } },
};

int cocinero_main(int num, int semid)
{
	struct cocinero *cocinero=&cocineros[num]; /* = cocineros+num */
	cocinero->sembuf[0].sem_op=-1;
	cocinero->sembuf[0].sem_flg=0;
	cocinero->sembuf[1].sem_op=-1;
	cocinero->sembuf[1].sem_flg=0;

	cocinero->sembuf[2].sem_num=4;
	cocinero->sembuf[2].sem_op=-1;
	cocinero->sembuf[2].sem_flg=0;

	semop(semid, cocinero->sembuf, 3);
	printf("  cocinero \"%s\" laburando ... \n", cocinero->name);
	usleep(300000);
	printf("  cocinero fin.\n");
	proveedor_liberar(semid);
	printf("  cocinero libera proveedor.\n");
	return 0;
}
int cocinero_crea(int num, int semid) 
{
	int pid;
	if ((pid=fork())==0) {
		int ret;
		ret=cocinero_main(num,semid);
		_exit(ret);
	}
	return pid;
}
