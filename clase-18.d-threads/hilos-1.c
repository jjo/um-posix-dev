/* $Id: hilos-1.c,v 1.1 2002/09/16 21:10:14 jjo Exp $ */
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

struct hilo_arg {
	int num;
	int n_iter;
	char *id;
};

void * hilo(void *arg)
{
	int i;
	struct hilo_arg *ha=arg;
	for (i=0;i<100;i++) {
		printf("%d", ha->num);
		sched_yield();
	}
	pthread_exit(NULL);
}
#define MAX_HILOS 100
int main(int argc, const char *argv[]) 
{
	int i;
	
	pthread_t hilos[MAX_HILOS];
	struct hilo_arg hilo_args[MAX_HILOS];

	char name[10];
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

	fprintf(stderr, "n_hilos=%d\n", n_hilos);
	
	for (i=0; i<n_hilos;i++) {
		sprintf(name, "hilito%02d", i);
		hilo_args[i].num=i;
		hilo_args[i].id=strdup(name);
		if (pthread_create(&hilos[i], NULL, hilo, (void*)&hilo_args[i]))
			perror("pthread_create()");
	}

	for (i=0; i<n_hilos;i++) {
		while(pthread_join(hilos[i], NULL));
	}

	return 0;
}
