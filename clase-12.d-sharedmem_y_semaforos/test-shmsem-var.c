/* $Id: test-shmsem-var.c,v 1.11 2005/08/26 21:14:27 jjo Exp $ */
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

#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "ipclib.h"

#define SHM_KEY (ftok(".",0))
#define SEM_KEY (ftok(".",0))

/* numero del semaforo dentro del set */
#define SEM_de_VARIABLE	0
#define SEM_de_ARRANQUE	1
#define CANT_SEMS	2

#define MAX_HIJOS 150

/* 
 * variable global que indica si usar semaforos para
 * sincronizar o no 
 */

int use_sem=1;

const char *nombre_prog;
/* 
 * hilo que itera n_iter veces  sumando 1 al "entero" comun 
 * que esta' en sharedmem
 */
void hilo(int *int_p, int sem_id, int n_iter, char *id)
{
	int a;
	int i;

	mi_sema_down(sem_id, SEM_de_ARRANQUE);

	for (i=0;i<n_iter;i++) {
/*%*/		if (use_sem) mi_sema_down_flags(sem_id, SEM_de_VARIABLE, 1, SEM_UNDO);

		a=*int_p;
		a++;
		if (id) write(2,id,strlen(id));
		*int_p=a;

/*%*/		if (use_sem) mi_sema_up_flags(sem_id, SEM_de_VARIABLE, 1, SEM_UNDO);
	}
}
void usage(void) {
	fprintf(stderr, "ERROR: uso: %s [-n] n_hijos n_iter \n", nombre_prog);
	exit(1);
}
int main(int argc, char *const argv[]) 
{
	int *int_p;		/* puntero a sharedmem, sobre el cual operaran los hijos */
	int i;
	char name[10];		/* nombre de c/hijo */
	key_t shm_key=SHM_KEY;	/* comodidad: uso la misma KEY para sharedmem y ... */
	key_t sem_key=SEM_KEY;	/* ... set de semaforos */
	int sem_id;	/* id del set de semaforos */
	int n_hijos;	/* cant de hijos a lanzar */
	int n_iter;	/* cant de iteraciones de c/hilo */

	nombre_prog = argv[0];

	/* Validacion ... */

	while((i=getopt(argc, argv, "n"))>=0) {
		switch(i) {
			case 'n': use_sem=0;break;
			default: usage();
		}
	}
	argc-=(optind-1);
	argv+=(optind-1);
	if (argc!=3) {
		usage();
	}
	if (    (n_hijos=atoi(argv[1])) <= 0  || 
		(n_iter=atoi(argv[2]))  <= 0  || 
		 n_hijos > MAX_HIJOS ) {
		fprintf(stderr, "ERROR: argumento(s) no valido(s) "
			"n_hijos=%d, n_iter=%d\n",
				n_hijos, n_iter);

		return 255;

	}
	fprintf(stderr, "use_sem=%d n_hijos=%d, n_iter=%d\n",
			use_sem, n_hijos, n_iter);
	
	/* creacion del segmento de shared mem */
	int_p=shm_create(shm_key, 4096, IPC_CREAT|0666, 0);
	if (!int_p) 	return 1;

	/* creacion del set de mi_semaforos */
	sem_id=mi_sema_create(sem_key, CANT_SEMS, IPC_CREAT|0666);
	if (sem_id<0)	return 1;


	/* 
	 * lanza los hijos, los cuales quedara'n durmiendo en el 
	 * semaforo 
	 */
	printf("\ntotal = %d (%d*%d)\n", n_hijos*n_iter, n_hijos, n_iter);

	for (i=0; i<n_hijos;i++) {
		/* "nombre" de c/hijo */
		sprintf(name, "\r%02d", i);
		if (fork()==0) { 
			/* hijo */
			hilo(int_p, sem_id, n_iter, name); exit(0); 
		}
	}

	fprintf(stderr, "%d hijos creados... esperando Enter->", n_hijos);
	getchar();write(1,"\n",1);

	/* inicializo el "entero compartido", y el semaforo de la variable */
	*int_p=0;
	mi_sema_up(sem_id, SEM_de_VARIABLE);

	/* 
	 * se~aliza n_hijos simulta'neamente, aqui' n_hijos es la cant. de 
	 * "recursos" a sumar al semaforo 
	 */
	mi_sema_up_flags(sem_id, SEM_de_ARRANQUE, n_hijos, 0);

	/* espera la muerte de todos los hijos */
	while(waitpid(-1, NULL, 0)>0);

	printf("\ntotal'= %d\n", *int_p);
	return 0;
}
