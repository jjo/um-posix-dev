/* $Id: hilos-3-condvars.c,v 1.3 2002/10/03 01:31:53 jjo Exp $ */
/*
 * Objetivo: POSIX threads: Mostrar el uso de "condition variables" en 
 * Implementacion:
 * 	Se lanzan dos threads: 1 que espera que un contador llegue a un
 * 	determinado n�mero y otra que incrementa dicho contador a intervalo
 * 	regular.
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

/*
 * 	la "condition variables" son una _generalizaci�n_ de los
 * 	sem�foros: es posible espera CUALQUIER CONDICI�N (y no 
 * 	solamente que el valor del sem�foro V>0).
 * 	Constan de 2 elementos:
 * 	- pthread_cont_t cond    
 * 	   provee "se�alizaci�n" v�a primitivas sleep/wakeup:
 * 	      pthread_cond_wait(&c, &m):  "sleep"
 * 	      pthread_cond_signal(&c):    "wakeup"
 * 	      
 * 	- pthread_mutex_t cond_mutex  
 * 	   mutex utilizado para proveer exclusi�n mutua durante
 * 	   la evaluaci�n/se�alizaci�n de la condici�n.
 */
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
/* 	
 * 	En realidad, _cualquier_ condici�n es posible,
 * 	en este caso simplemente una variable
 */
int cond_var=0;

/*
 * El thread asociado a esta funci�n tiene como misi�n "disparar"
 * (pthread_cond_signal) peri�dicamente 
 */
#define PERIOD_usec	1000000/5
void *hilo_dispara(void *data)
{
	while(1) {
		pthread_mutex_lock(&cond_mutex);
		cond_var++;
		pthread_mutex_unlock(&cond_mutex);
		pthread_cond_signal(&cond);
		printf("TICK: cond_var=%d -> ", cond_var);
		usleep(PERIOD_usec);
	}
	return NULL;
}

/*
 * El thread asociado a esta funci�n tiene como misi�n "esperar"
 * la condici�n, en este caso un determinado valor de "cond_var"
 */
void *hilo_espera(void *data)
{
	pthread_mutex_lock(&cond_mutex);
	while (cond_var < 10) {
		printf(".\n");
		pthread_cond_wait(&cond, &cond_mutex);
	}
	pthread_mutex_unlock(&cond_mutex);
	printf("LISTO\n");
	return NULL;
}

int main(void) {
	pthread_t tid_espera, tid_dispara;	/* IDs de threads */
	/*	Creaci�n de threads:
	 *	- hilo_espera: el que espera la condici�n (esperando se�al)
	 *	- hilo_dispara: el que cambia la condici�n y se�aliza 
	 */
	pthread_create ( &tid_espera, NULL, hilo_espera, NULL);
	pthread_create ( &tid_dispara, NULL, hilo_dispara, NULL);
	pthread_join ( tid_espera, NULL);
	return 0;
}
