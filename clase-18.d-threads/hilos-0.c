/* $Id: hilos-0.c,v 1.2 2003/10/10 21:24:17 jjo Exp $ */
/*
 * Objetivo: POSIX threads: Mostrar la creaci�n de threads y la espera de finalizaci�n
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

#define N_HILOS 10

void *hilo (void *arg);

int main(void)
{
	int i;
/*%%*/	pthread_t hilos[N_HILOS];	/* para guardar todos los ID de threads  */
	/* int hilo_args[N_HILOS];	*/

	fprintf(stderr, "n_hilos=%d\n", N_HILOS);

	/* Lanza los threads */
	for (i=0; i<N_HILOS;i++) {
		/* hilo_args[i]=i; */

		/* LA proxima linea tiene un BUG ! */
/*%%*/		if (pthread_create(&hilos[i], NULL, hilo, (void*)&i)) 
			perror("pthread_create()");
	}

	/* espera por finalizacio'n de TODOS */
	for (i=0; i<N_HILOS;i++) {
/*%%*/		while(pthread_join(hilos[i], NULL));
	}

	return 0;
}
void * hilo(void *arg)
{
	char buf[256];
	int n;
	pthread_t tid;
/*%%*/	tid=pthread_self();	/* pthread_self: devuelve el THREAD ID */

	usleep(random()%20000);
	n=snprintf(buf,sizeof(buf),"tid=%ld arg=%d\n", (long)tid, *(int*)arg);
	write(STDOUT_FILENO, buf, n);
/*%%*/	pthread_exit(NULL);	/* pthread_exit: termina y retorna exit_val */
}