/* 
 * $Id: hilo-main.c,v 1.3 2003/10/30 22:01:48 jjo Exp $
 *
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 * License: GPLv2
 *
 * Despliega n_hilos, cada uno de los cuales realiza n_loops 
 * incrementando una variable GLOBAL protegida con "algun"
 * mecanismo de "bloqueo de concurrencia" o "zona critica".
 *
 * El mecanismo NO esta' aqui, sino que depende de la implementacion
 * que se vincule en momento de 'link'. 
 *
 * Dicha implementacio'n debe proveer (ver libjj.h):
 *	void *jj_critic_new(void);
 *	int jj_critic_on(void *);
 *	int jj_critic_off(void *);
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include "libjj.h"

#define MAX_HILOS 100
#define min(a,b) ((a)<=(b)? (a) : (b))

struct var_protegida {
	void *lock;	/* la _implementacio'n_ dependera' de la lib. linkeada */
	unsigned valor;
};

int n_loops=0;	/* cant. de loops por thread */

#define EXIT_IF(cond, msg)  do { if (cond) {perror(msg);exit(1);} } while(0);
void *incr(void *);
int main(int argc , char *argv[])
{
	int i, n_hilos;
	pthread_t tid[MAX_HILOS];
	struct var_protegida v;

	/* procesado de argumentos de linea de comando */
	if (argc!=3) {
		fprintf(stderr, "uso: %s <n_loops> <n_hilos>\n"
				" ej; %s 1000 20\n",
				argv[0], argv[0]);
		return 1;
	}
	n_loops=atoi(argv[1]);
	n_hilos=atoi(argv[2]);
	n_hilos=min(n_hilos, MAX_HILOS);

	/* setup de la variable protegida: valor y lock */
	v.valor=0;
/*%%*/	v.lock=jj_critic_new();	

	EXIT_IF(!v.lock, "critic_new() retorno NULL");

	printf("Desplegando %d hilos * %d loops\n", n_hilos, n_loops);

	/* enciendo el lock, de tal manera que los hilos desplegados
	   quedan TODOS bloqueados */
/*%%*/	jj_critic_on(v.lock);

	/* despliego n_hilos */
	for (i=0; i< n_hilos; i++) {
		EXIT_IF(pthread_create(&tid[i], NULL, incr, &v)!=0,
			"problema en pthread_create()");
	}

	/* apago el lock ... */
/*%%*/	jj_critic_off(v.lock);

	/* espero la finalizacio'n de todos los hilos */
	for (i=0; i< n_hilos; i++) {
		pthread_join(tid[i], NULL);
	}
	printf("valor=%d\n", v.valor);
	
	return 0;
}

void *incr(void *var) {
	int i=n_loops;
	struct var_protegida *vp=var;

	//printf("dentro ... de thread %lu\n", pthread_self());
	while(i--) {
		/* protejo la variable mientras la modifico */
/*%%*/		jj_critic_on(vp->lock);
		vp->valor++;
/*%%*/		jj_critic_off(vp->lock);
	}
	return NULL;
}
