/* $Id: hilos-2.c,v 1.1 2002/09/27 23:37:09 jjo Exp $ */
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

/* Probar con DO_SYNC en 0 o' 1 */
#ifndef DO_SYNC
#define DO_SYNC 0
#endif

#ifndef USE_MUTEX
#define USE_MUTEX 1
#endif

#define SEMA_VAR 0
#define SEMA_ARRANQUE 1
#define MAX_HILOS 50

int elentero;

struct hilo_arg {
	int num;
	int n_iter;
	sem_t *sem;
	pthread_mutex_t *mutex;
};

void * hilo(void *arg)
{
	int i,n;
	char buf[256];
	int a;
	struct hilo_arg *ha=arg;
#if DO_SYNC
	if (sem_wait(&ha->sem[SEMA_ARRANQUE])) {
		perror("hilo: sem_wait()");
		pthread_exit(NULL);
	}
#endif /* DO_SYNC */
	for (i=0;i<ha->n_iter;i++) {
#if DO_SYNC
#if USE_MUTEX
		pthread_mutex_lock(ha->mutex);
#else
		sem_wait(&ha->sem[SEMA_VAR]);
#endif /* USE_MUTEX */
#endif /* DO_SYNC */
		a=elentero;
		a++;
		n=snprintf(buf,sizeof(buf),"%02d-\b\b\b", ha->num);
		write(1, buf, n);
		elentero=a;

#if DO_SYNC
#if USE_MUTEX
		pthread_mutex_unlock(ha->mutex);
#else
		sem_post(&ha->sem[SEMA_VAR]);
#endif /* USE_MUTEX */
#endif /* DO_SYNC */
	}
	pthread_exit(NULL);
}
int main(int argc, const char *argv[]) 
{
	int i;
	pthread_t hilos[MAX_HILOS];
	struct hilo_arg hilo_args[MAX_HILOS];
	char name[10];
#if DO_SYNC
	sem_t sem[2];
#endif /* DO_SYNC */
#if USE_MUTEX
	pthread_mutex_t mutex;
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
	
#if DO_SYNC
	sem_init(&sem[SEMA_VAR], 0, 1);
	sem_init(&sem[SEMA_ARRANQUE], 0, 0);
#endif /* DO_SYNC */
#if USE_MUTEX
	pthread_mutex_init(&mutex, NULL);
#endif

	elentero=0;
	printf("\ntotal = %d (%d*%d)\n", n_hilos*n_iter, n_hilos, n_iter);
	for (i=0; i<n_hilos;i++) {
		hilo_args[i].num=i;
		sprintf(name, "\r%02d", i);
#if DO_SYNC
		hilo_args[i].sem=sem;
#if USE_MUTEX
		hilo_args[i].mutex=&mutex;
#endif
#endif /* DO_SYNC */
		hilo_args[i].n_iter=n_iter;
		if (pthread_create(&hilos[i], NULL, hilo, (void*)&hilo_args[i]))
			perror("pthread_create()");
	}
#if DO_SYNC /* compila condicionalmente */
	fprintf(stderr, "%d hilos creados... esperando Enter->", n_hilos);
	getchar();write(1,"\n",1);
	for (i=0; i<n_hilos;i++) {
		sem_post(&sem[SEMA_ARRANQUE]);
	}
#endif /* DO_SYNC */

	for (i=0; i<n_hilos;i++) {
		while(pthread_join(hilos[i], NULL));
	}

	printf("\ntotal'= %d\n", elentero);
	return 0;
}
