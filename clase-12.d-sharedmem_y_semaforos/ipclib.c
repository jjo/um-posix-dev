/* $Id: ipclib.c,v 1.3 2002/06/21 18:40:41 jjo Exp $ */
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
#define SEM_MAX 10
/* crear un area de memoria compartida */
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

int sema_create(key_t key, size_t n_sems, int getflags)
{
	int sem_id;
	unsigned short vals[SEM_MAX];
	union semun arg;

	if(n_sems>=SEM_MAX) {
		fprintf(stderr, "ERROR: n_sems=%d>SEM_MAX\n",
				n_sems);
		return -1;
	}
	sem_id=semget(key, n_sems, getflags);
	if (sem_id ==-1) { perror ("semget"); return -1;}
	/* reset */
	memset(vals, 0, sizeof(vals));
	arg.array=vals;
	semctl(sem_id,n_sems,SETALL,arg);
	return sem_id;
}
int sema_up_flg(int sem_id, int sem_num, int delta, int flg)
{
	struct sembuf semops[1];
	semops[0].sem_num=sem_num;
	semops[0].sem_op=delta;
	semops[0].sem_flg=flg;
	
	return semop(sem_id, semops, 1);
}