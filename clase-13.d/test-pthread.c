/* $Id: test-pthread.c,v 1.2 2002/06/21 21:42:25 jjo Exp $ */
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

/* Probar con USE_SEM en 0 o' 1 */
#ifndef USE_SEM
#define USE_SEM 1
#endif

#ifndef USE_MUTEX
#define USE_MUTEX 1
#endif

#define SEMA_VAR 0
#define SEMA_ARRANQUE 1
#define MAX_HILOS 50

struct hilo_arg {
	int *int_p;
	sem_t *sem;
	pthread_mutex_t *mutex;
	int n_iter;
	char *id;
};

void * hilo(void *arg)
{
	int a;
	int i;
	struct hilo_arg *hilo_arg=arg;
#if USE_SEM
	if (sem_wait(&hilo_arg->sem[SEMA_ARRANQUE])) {
		perror("hilo: sem_wait()");
		pthread_exit(NULL);
	}
#endif /* USE_SEM */
	for (i=0;i<hilo_arg->n_iter;i++) {
#if USE_SEM
#if USE_MUTEX
		pthread_mutex_lock(hilo_arg->mutex);
#else
		sem_wait(&hilo_arg->sem[SEMA_VAR]);
#endif /* USE_MUTEX */
#endif /* USE_SEM */
		a=*hilo_arg->int_p;
		a++;
		if (hilo_arg->id) write(2,hilo_arg->id,strlen(hilo_arg->id));
		*hilo_arg->int_p=a;

#if USE_SEM
#if USE_MUTEX
		pthread_mutex_unlock(hilo_arg->mutex);
#else
		sem_post(&hilo_arg->sem[SEMA_VAR]);
#endif /* USE_MUTEX */
#endif /* USE_SEM */
	}
	pthread_exit(NULL);
}
int main(int argc, const char *argv[]) 
{
	int elentero;
	int i;
	pthread_t hilos[MAX_HILOS];
	struct hilo_arg hilo_args[MAX_HILOS];
	pthread_attr_t attr;
	char name[10];
#if USE_SEM
	sem_t sem[2];
#endif /* USE_SEM */
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
	
#if USE_SEM
	sem_init(&sem[SEMA_VAR], 0, 1);
	sem_init(&sem[SEMA_ARRANQUE], 0, 0);
#endif /* USE_SEM */
#if USE_MUTEX
	pthread_mutex_init(&mutex, NULL);
#endif

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	elentero=0;
	printf("\ntotal = %d (%d*%d)\n", n_hilos*n_iter, n_hilos, n_iter);
	for (i=0; i<n_hilos;i++) {
		sprintf(name, "\r%02d", i);
		hilo_args[i].int_p=&elentero;
#if USE_SEM
		hilo_args[i].sem=sem;
#if USE_MUTEX
		hilo_args[i].mutex=&mutex;
#endif
#endif /* USE_SEM */
		hilo_args[i].n_iter=n_iter;
		hilo_args[i].id=strdup(name);
		if (pthread_create(&hilos[i], &attr, hilo, (void*)&hilo_args[i]))
			perror("pthread_create()");
	}
	pthread_attr_destroy(&attr);
#if USE_SEM /* compila condicionalmente */
	fprintf(stderr, "%d hilos creados... esperando Enter->", n_hilos);
	getchar();write(1,"\n",1);
	for (i=0; i<n_hilos;i++) {
		sem_post(&sem[SEMA_ARRANQUE]);
	}
#endif /* USE_SEM */

	for (i=0; i<n_hilos;i++) {
		while(pthread_join(hilos[i], NULL));
	}

	printf("\ntotal'= %d\n", elentero);
	return 0;
}
