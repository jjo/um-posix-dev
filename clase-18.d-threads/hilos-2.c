/* $Id: hilos-2.c,v 1.2 2002/10/06 22:51:49 jjo Exp $ */
/*
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

#define MAX_HILOS 100

/*
 * DO_SYNC indica si se usara'n primitivas de sync 
 * para acceso a la variable global 
 */

/* Probar con DO_SYNC en 0 o' 1 */
#ifndef DO_SYNC
#define DO_SYNC 1
#endif

/* 
 * Solo si DO_SYNC en 1
 * 
 * USE_SEM 1 -> semaforos
 * USE_SEM 0 -> mutexes
 */
#ifndef USE_SEM
#define USE_SEM 0
#endif

int elentero;

/* DATA para la funcion del thread */
struct hilo_arg {
	int num;
	int n_iter;
	sem_t *sem_varp, *sem_arranquep;
	pthread_mutex_t *mutex;
};

/* FUNCION del thread */
void * hilo(void *arg)
{
	int i,n;
	char buf[256];
	pthread_t tid;
	int a;
	struct hilo_arg *ha=arg;
	tid=pthread_self();

	n=snprintf(buf,sizeof(buf),"tid=%ld ", (long)tid);
	write(1, buf, n);
	
	if (sem_wait(ha->sem_arranquep)) {
		perror("hilo: sem_wait()");
		pthread_exit(NULL);
	}
	for (i=0;i<ha->n_iter;i++) {
#if DO_SYNC
#if USE_SEM
		sem_wait(ha->sem_varp);
#else
		pthread_mutex_lock(ha->mutex);
#endif
#endif
		a=elentero;
		a++;
		n=snprintf(buf,sizeof(buf),"%02d-\b\b\b", ha->num);
		write(1, buf, n);
		elentero=a;
#if DO_SYNC
#if USE_SEM
		sem_post(ha->sem_varp);
#else
		pthread_mutex_unlock(ha->mutex);
#endif
#endif
	}
	pthread_exit(NULL);
}
int main(int argc, const char *argv[]) 
{
	int i;
	
	/* para guardar todos los ID de threads (opcional) */
	pthread_t hilos[MAX_HILOS];
	/* ... y los DATA de c/thread */
	struct hilo_arg hilo_args[MAX_HILOS];
	char name[10];
	sem_t sem_arranque;
#if DO_SYNC
#if USE_SEM
	sem_t sem_var;
#else
	pthread_mutex_t mutex;
#endif
#endif
	int n_hilos;   /* cant de hilos a lanzar */
	int n_iter;    /* cant de iteraciones de c/hilo */

	if (argc!=3) {
		fprintf(stderr, "ERROR: uso: %s n_hilos n_iter \n", argv[0]);
		return 255;
	}
	if (    (n_hilos=atoi(argv[1])) <= 0  || n_hilos > MAX_HILOS ||
		(n_iter=atoi(argv[2]))  <= 0  ) {
		fprintf(stderr, "ERROR: argumento(s) no valido(s) "
			"n_hilos=%d, n_iter=%d\n",
				n_hilos, n_iter);

		return 255;

	}
	fprintf(stderr, "n_hilos=%d, n_iter=%d\n",
				n_hilos, n_iter);
	
	sem_init(&sem_arranque, 0, 0);
#if DO_SYNC
#if USE_SEM
	sem_init(&sem_var, 0, 1);
#else
	pthread_mutex_init(&mutex, NULL);
#endif
#endif

	/* lanzado de los threads */
	fprintf(stderr, "n_hilos=%d\n", n_hilos);
	elentero=0;
	printf("\ntotal = %d (%d*%d)\n", n_hilos*n_iter, n_hilos, n_iter);
	for (i=0; i<n_hilos;i++) {
		hilo_args[i].num=i;
		sprintf(name, "\r%02d", i);
		hilo_args[i].sem_arranquep=&sem_arranque;
#if DO_SYNC
#if USE_SEM
		hilo_args[i].sem_varp=&sem_var;
#else
		hilo_args[i].mutex=&mutex;
#endif
#endif
		hilo_args[i].n_iter=n_iter;
		if (pthread_create(&hilos[i], NULL, hilo, (void*)&hilo_args[i]))
			perror("pthread_create()");
	}
	fprintf(stderr, "%d hilos creados... esperando Enter->", n_hilos);
	getchar();write(1,"\n",1);
	for (i=0; i<n_hilos;i++) {
		sem_post(&sem_arranque);
	}

	/* espera por finalizacio'n de TODOS */

	for (i=0; i<n_hilos;i++) {
		while(pthread_join(hilos[i], NULL));
	}

	printf("\ntotal'= %d\n", elentero);
	return 0;
}