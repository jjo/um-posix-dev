/* $Id: ipclib.h,v 1.4 2003/06/13 21:19:13 jjo Exp $ */
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

/* ver semctl(2) */
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

void *shm_create(key_t key, size_t size, int getflags, int atflags);

int mi_sema_create(key_t key, size_t n_sems, int getflags);
int mi_sema_up_flags(int sem_id, int sem_num, int delta, int flags);

/* funciones inline: "casi" macros, pero con chequeo de tipo de args */
static __inline__ int mi_sema_down_flags(int id, int num, int delta, int flags)
{
	return mi_sema_up_flags(id, num, -delta, flags);
}
static __inline__ int mi_sema_up(int id, int num)
{
	return mi_sema_up_flags(id, num, 1, 0);
}
static __inline__ int mi_sema_down(int id, int num)
{
	return mi_sema_up_flags(id, num, -1, 0);
}

