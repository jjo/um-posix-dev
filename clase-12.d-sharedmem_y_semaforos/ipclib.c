/* $Id: ipclib.c,v 1.4 2003/06/13 21:19:13 jjo Exp $ */
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
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#include "ipclib.h"

/* 
 * Crea un area de memoria compartida 
 */
void *shm_create(key_t key, size_t size, int getflags, int atflags)
{
	int shm_id;
	void *ptr;
	shm_id=shmget(key, size, getflags);
	if (shm_id ==-1) { perror ("shmget");return NULL;}
	ptr=shmat(shm_id, NULL, atflags);
	if (ptr ==(void *)-1) { perror ("shmat");return NULL;}
	return ptr;
}

/* 
 * Crea e inicializa un set de mi_semaforos
 */ 
int mi_sema_create(key_t key, size_t n_sems, int getflags)
{
	int sem_id;
	const char *ouch="";
	union semun semctl_arg;
	/* typeof es una extension de GCC ... muy co'moda x cierto :) */
	typeof(semctl_arg.array) valores=NULL;

	/* contendra' los valores para c/mi_semaforo */
	valores=malloc(n_sems * sizeof *valores);
	if (!valores)    { ouch="malloc()"; goto err; }

	sem_id=semget(key, n_sems, getflags);
	if (sem_id ==-1) { ouch="semget()"; goto err; }

	/* pongo todos los valores en 0 */
	memset(valores, 0, sizeof(valores));
	
	/* preparo el argumento para semctl */
	semctl_arg.array=valores;

	/* inicializacion del set */
	if (semctl(sem_id, n_sems, SETALL, semctl_arg)==-1) 
	{ ouch="semctl()"; goto err; }

	free(valores);
	return sem_id;
err:
	if (valores) free (valores);
	perror(ouch);
	return -1;
}
/* 
 * Hace una operacion sobre el mi_semaforo numero sem_num 
 * para
 * 	delta=1  -> UP
 * 	delta=-1 -> DOWN
 */
int mi_sema_up_flags(int sem_id, int sem_num, int delta, int flags)
{
	struct sembuf semops[1];
	semops[0].sem_num=sem_num;
	semops[0].sem_op=delta;
	semops[0].sem_flg=flags;
	
	return semop(sem_id, semops, 1);
}
