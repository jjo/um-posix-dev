/* 
 * $Id: hilo-main.c,v 1.2 2003/10/30 21:07:39 jjo Exp $
 *
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 * License: GPLv2
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include "libjj.h"

#define min(a,b) ((a)<=(b)? (a) : (b))
struct var_protegida {
	void *lock;
	unsigned valor;
};

#define MAX_HILOS 100
int n_loops=0;
void *incr(void *var) {
	int i=n_loops;
	struct var_protegida *vpp=var;
	//printf("adentro ... \n");
	while(i--) {
		jj_critic_on(vpp->lock);
		vpp->valor++;
		jj_critic_off(vpp->lock);
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
	if(!(vp.lock=jj_critic_new())) {
		fprintf(stderr, "critic_new() retorno NULL\n");
		return 2;
	}
	printf("Desplegando %d hilos * %d loops\n", n_hilos, n_loops);
	jj_critic_on(vp.lock);
	for (i=0; i< n_hilos; i++) {
		pthread_create(&tid[i], NULL, incr, &vp);
	}
	jj_critic_off(vp.lock);
	for (i=0; i< n_hilos; i++) {
		pthread_join(tid[i], NULL);
	}
	printf("valor=%d\n", vp.valor);
	
	return 0;
}
