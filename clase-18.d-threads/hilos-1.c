/* $Id: hilos-1.c,v 1.7 2004/09/10 16:55:35 jjo Exp $ */
/*
 * Objetivo: POSIX threads: Mostrar la creación de threads y la espera de finalización
 * 
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

#define MAX_HILOS 100

/* DATA para la funcion del thread */
struct hilo_arg {
	int num;
	int n_iter;
};

void * hilo(void *arg);

int main(int argc, const char *argv[]) 
{
	int i;
	
	/* para guardar todos los ID de threads (opcional) */
	pthread_t hilos[MAX_HILOS];
	/* ... y los DATA de c/thread */
	struct hilo_arg hilo_args[MAX_HILOS];
	int  n_hilos;

	if (argc!=2) {
		fprintf(stderr, "ERROR: uso: %s n_hilos \n", argv[0]);
		return 255;
	}
	if (    (n_hilos=atoi(argv[1])) <= 0  || n_hilos > MAX_HILOS) {
		fprintf(stderr, "ERROR: argumento(s) no valido(s) "
			"n_hilos=%d",
				n_hilos);
		return 255;
	}

	/* lanzado de los threads */
	fprintf(stderr, "n_hilos=%d\n", n_hilos);
	for (i=0; i<n_hilos;i++) {
		hilo_args[i].num=i;
		hilo_args[i].n_iter=20;
		if (pthread_create(&hilos[i], NULL, hilo, (void*)&hilo_args[i]))
			perror("pthread_create()");
	}

	/* espera por finalizacio'n de TODOS */

	for (i=0; i<n_hilos;i++) {
		while(pthread_join(hilos[i], NULL));
	}

	return 0;
}
/* FUNCION del thread */
void * hilo(void *arg)
{
	int i,n;
	char buf[256];
	pthread_t tid;
	struct hilo_arg *ha=arg;
	tid=pthread_self();

	n=snprintf(buf,sizeof(buf),"tid=%ld \n", (long)tid);
	write(STDOUT_FILENO, buf, n);
	
	for (i=0;i<ha->n_iter;i++) {
		/* dormir rando'micamente */
		usleep(random()%100000);

		/* evitar usar stdio en threads */
		n=snprintf(buf,sizeof(buf),"%02d-", ha->num);
		write(STDOUT_FILENO, buf, n);
	}
	pthread_exit(NULL);
}
