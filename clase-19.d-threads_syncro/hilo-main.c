#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include "locklib.h"

#define min(a,b) ((a)<=(b)? (a) : (b))
struct var_protegida {
	void *lockeador;
	unsigned valor;
};

#define MAX_HILOS 100
int n_loops=0;
void *incr(void *var) {
	int i=n_loops;
	struct var_protegida *vpp=var;
	//printf("adentro ... \n");
	while(i--) {
		locklib_lock(vpp->lockeador);
		vpp->valor++;
		locklib_unlock(vpp->lockeador);
	}
	return NULL;
}
int main(int argc , char *argv[])
{
	int i, n_hilos;
	pthread_t tid[MAX_HILOS];
	struct var_protegida vp;
	if (argc!=3) {
		fprintf(stderr, "uso: %s <n_loops> <n_hilos>\n",
				argv[0]);
		return 1;
	}
	n_loops=atoi(argv[1]);
	n_hilos=atoi(argv[2]);
	n_hilos=min(n_hilos, MAX_HILOS);
	vp.valor=0;
	if(!(vp.lockeador=locklib_new())) {
		fprintf(stderr, "locklib_new() retorno NULL\n");
		return 2;
	}
	printf("Desplegando %d hilos * %d loops\n", n_hilos, n_loops);
	locklib_lock(vp.lockeador);
	for (i=0; i< n_hilos; i++) {
		pthread_create(&tid[i], NULL, incr, &vp);
	}
	locklib_unlock(vp.lockeador);
	for (i=0; i< n_hilos; i++) {
		pthread_join(tid[i], NULL);
	}
	printf("valor=%d\n", vp.valor);
	
	return 0;
}
