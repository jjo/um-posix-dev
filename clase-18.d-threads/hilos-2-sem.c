/* $Id: hilos-2-sem.c,v 1.2 2004/10/01 22:05:14 jjo Exp $ */
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

/* variable global "sincronizada" */
struct {
	int entero;
	sem_t semaforo;
} LA_Variable;

void usage(int, int);
#define MAX_HILOS 100
const char *nombre_prog;
/*
 * do_sync indica si se usara'n primitivas de sync 
 * para acceso a la variable global 
 */
int do_sync=1;

/* DATA para la funcion del thread */
struct hilo_arg {
	int num;
	int n_iter;
	sem_t *sem_arranquep;
};

/* FUNCION del thread */
void * hilo(void *arg)
{
	int i,n;
	char buf[256];
	int a;
	struct hilo_arg *ha=arg;
	
	if (sem_wait(ha->sem_arranquep)) {
		perror("hilo: sem_wait()");
		pthread_exit(NULL);
	}
	for (i=0;i<ha->n_iter;i++) {
		if (do_sync) {
			sem_wait(&LA_Variable.semaforo);
		}
		a=LA_Variable.entero;
		a++;
		n=snprintf(buf,sizeof(buf),"%02d-\b\b\b", ha->num);
		write(STDOUT_FILENO, buf, n);
		LA_Variable.entero=a;
		if (do_sync) {
			sem_post(&LA_Variable.semaforo);
		}
	}
	pthread_exit(NULL);
}
int main(int argc, char **argv) 
{
	int i;
	
	/* para guardar todos los ID de threads (opcional) */
	pthread_t hilos[MAX_HILOS];
	/* ... y los DATA de c/thread */
	struct hilo_arg hilo_args[MAX_HILOS];
	char name[10];
	sem_t sem_arranque;

	int n_hilos;   /* cant de hilos a lanzar */
	int n_iter;    /* cant de iteraciones de c/hilo */

	nombre_prog = argv[0];

	/* Validacion ... */

	while((i=getopt(argc, argv, "n"))>=0) {
		switch(i) {
			case 'n': do_sync=0;break;
			default: usage(n_hilos, n_iter);
		}
	}
	argc-=(optind-1);
	argv+=(optind-1);
	if (argc!=3) {
		fprintf(stderr, "ERROR: uso: %s [-n]  n_hilos n_iter \n", argv[0]);
		return 255;
	}
	if (    (n_hilos=atoi(argv[1])) <= 0  || n_hilos > MAX_HILOS ||
		(n_iter=atoi(argv[2]))  <= 0  ) {
		usage(n_hilos, n_iter);

	}
	fprintf(stderr, "do_sync=%d n_hilos=%d, n_iter=%d\n",
				do_sync, n_hilos, n_iter);
	
	sem_init(&sem_arranque, 0, 0);
	if (do_sync) {
		sem_init(&LA_Variable.semaforo, 0, 1);
	}

	/* lanzado de los threads */
	fprintf(stderr, "n_hilos=%d\n", n_hilos);
	LA_Variable.entero=0;
	printf("\ntotal = %d (%d*%d)\n", n_hilos*n_iter, n_hilos, n_iter);
	for (i=0; i<n_hilos;i++) {
		hilo_args[i].num=i;
		sprintf(name, "\r%02d", i);
		hilo_args[i].sem_arranquep=&sem_arranque;
		hilo_args[i].n_iter=n_iter;
		if (pthread_create(&hilos[i], NULL, hilo, (void*)&hilo_args[i]))
			perror("pthread_create()");
	}
	fprintf(stderr, "%d hilos creados... esperando Enter->", n_hilos);
	getchar();write(STDOUT_FILENO,"\n",1);
	for (i=0; i<n_hilos;i++) {
		sem_post(&sem_arranque);
	}

	/* espera por finalizacio'n de TODOS */

	for (i=0; i<n_hilos;i++) {
		while(pthread_join(hilos[i], NULL));
	}

	printf("\ntotal'= %d\n", LA_Variable.entero);
	return 0;
}
void usage(int h, int i) {
	fprintf(stderr, "ERROR: argumento(s) no valido(s) "
			"(n_hilos=%d, n_iter=%d)\n",
			h, i);

	exit(255);
}
