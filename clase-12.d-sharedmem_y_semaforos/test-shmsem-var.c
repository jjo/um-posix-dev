/* $Id: test-shmsem-var.c,v 1.6 2002/06/21 21:42:25 jjo Exp $ */
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

/* Probar con USE_SEM en 0 o' 1 */
#ifndef USE_SEM
#define USE_SEM 1
#endif

/*
#define SHM_KEY 0x00240667
#define SEM_KEY 0x00240667
*/
#define SHM_KEY (ftok(".",0))
#define SEM_KEY (ftok(".",0))

#define SEMA_VAR 0
#define SEMA_ARRANQUE 1

#define MAX_HILOS 50

void hilo(int *int_p, int sem_id, int n_iter, char *id)
{
	int a;
	int i;
#if USE_SEM
	if (sema_down(sem_id, SEMA_ARRANQUE)<0) {
		perror("hilo: sem_down()");
		exit(1);
	}
#endif /* USE_SEM */
	for (i=0;i<n_iter;i++) {
#if USE_SEM
		sema_down_flg(sem_id, SEMA_VAR, 1, SEM_UNDO);
#endif /* USE_SEM */
		a=*int_p;
		a++;
		if (id) write(2,id,strlen(id));
		*int_p=a;

#if USE_SEM
		sema_up_flg(sem_id, SEMA_VAR, 1, SEM_UNDO);
#endif /* USE_SEM */
	}
}
int main(int argc, const char *argv[]) 
{
	int *int_p;
	int i;
	char name[10];
	key_t shm_key=SHM_KEY;
#if USE_SEM
	key_t sem_key=SEM_KEY;
#endif /* USE_SEM */
	int sem_id;
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
	

	
	int_p=shm_create(shm_key, 4096, IPC_CREAT|0666, 0);
	if (!int_p)
		return 1;

#if USE_SEM
	sem_id=sema_create(sem_key, 2, IPC_CREAT|0666);
	if (sem_id<0)
		return 1;
	sema_up(sem_id, SEMA_VAR);
#endif /* USE_SEM */

	*int_p=0;
	printf("\ntotal = %d (%d*%d)\n", n_hilos*n_iter, n_hilos, n_iter);
	for (i=0; i<n_hilos;i++) {
		sprintf(name, "\r%02d", i);
		if (fork()==0) { hilo(int_p, sem_id, n_iter, name); exit(0); }
	}
#if USE_SEM
	fprintf(stderr, "%d hilos creados... esperando Enter->", n_hilos);
	getchar();write(1,"\n",1);
	sema_up_flg(sem_id, SEMA_ARRANQUE, n_hilos, 0);	/* se~aliza hilos simulta'neamente */
#endif /* USE_SEM */

	while(waitpid(-1, NULL, 0)>0);

	printf("\ntotal'= %d\n", *int_p);
	return 0;
}
