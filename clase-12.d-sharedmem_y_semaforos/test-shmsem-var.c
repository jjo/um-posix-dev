/* $Id */
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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/signal.h>

#include "ipclib.h"

#define SHM_KEY 0x00240667
#define SEM_KEY 0x00240667

/* Probar con USE_SEM en 0 o' 1 */
#ifndef USE_SEM
#define USE_SEM 0
#endif

void hijo(int *int_p, int sem_id, int n_iter, char *id)
{
	int a;
	int i;
#if USE_SEM /* compila condicionalmente */
	if (sem_down(sem_id, 1)<0) {
		perror("hijo: sem_down()");
		exit(1);
	}
#endif /* USE_SEM */
	for (i=0;i<n_iter;i++) {
#if USE_SEM /* compila condicionalmente */
		sem_down_flg(sem_id, 0, 1, SEM_UNDO);
#endif /* USE_SEM */
		a=*int_p;
		a++;
		if (id) write(2,id,strlen(id));
		*int_p=a;

#if USE_SEM /* compila condicionalmente */
		sem_up_flg(sem_id, 0, 1, SEM_UNDO);
#endif /* USE_SEM */
	}
}
int main(int argc, const char *argv[]) 
{
	int *int_p;
	int i;
	char name[10];
	key_t shm_key=SHM_KEY;
#if USE_SEM /* compila condicionalmente */
	key_t sem_key=SEM_KEY;
#endif /* USE_SEM */
	int sem_id;
	int n_hijos;   /* cant de hijos a lanzar */
	int n_iter;    /* cant de iteraciones de c/hijo */

	if (argc!=3) {
		fprintf(stderr, "ERROR: uso: %s n_hijos n_iter \n", argv[0]);
		return 255;
	}
	if (    (n_hijos=atoi(argv[1])) <= 0  || n_hijos > 50 ||
		(n_iter=atoi(argv[2]))  <= 0  ) {
		fprintf(stderr, "ERROR: argumento(s) no valido(s) "
			"n_hijos=%d, n_iter=%d\n",
				n_hijos, n_iter);

		return 255;

	}
	fprintf(stderr, "n_hijos=%d, n_iter=%d\n",
				n_hijos, n_iter);
	

	
	int_p=shm_create(shm_key, 4096, IPC_CREAT|0666, 0);
	if (!int_p)
		return 1;
	*int_p=0;

#if USE_SEM /* compila condicionalmente */
	sem_id=sem_create(sem_key, 2, IPC_CREAT|0666);
	if (sem_id<0)
		return 1;
	sem_up(sem_id, 0);
#endif /* USE_SEM */

	srand(time(NULL));
	printf("\ntotal = %d (%d*%d)\n", n_hijos*n_iter, n_hijos, n_iter);

	for (i=1; i<=n_hijos;i++) {
		sprintf(name, "\r%02d", i);
		if (fork()==0) { hijo(int_p, sem_id, n_iter, name); exit(0); }
	}
#if USE_SEM /* compila condicionalmente */
	fprintf(stderr, "Enter->");
	getchar();
	sem_up_flg(sem_id, 1, n_hijos, 0);	/* se~aliza hijos simulta'neamente */
#endif /* USE_SEM */

	while(waitpid(-1, NULL, 0)>0);

	printf("\ntotal'= %d\n", *int_p);
	return 0;
}
