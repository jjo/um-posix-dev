/* 
 * $Id: libjj_sem.c,v 1.2 2003/10/30 21:07:39 jjo Exp $
 *
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 * License: GPLv2
 *
 * Implementacion de region critica con rwlock_t (Read/Write lock)
 * 
 * Se utiliza la operacion de RWlock (lock non-shared lock) para
 * crear exclusio'n mutua.
 *
 * NO es el propo'sito ti'pico de un RWlock, el cual esta'
 * orientado a proteger variables que pueden ser lei'das y/o
 * escritas desde distintas regiones cri'ticas.
 *
 * La ma's veloz en glibc-2.2+linux-2.4.
 * 
 */
#include <stdlib.h>
#include <semaphore.h>
#include "libjj.h"

void *jj_critic_new(void)
{
	sem_t *lk=malloc(sizeof (sem_t));
	if (!lk) return NULL;
	sem_init(lk, 0, 1);
	return lk;
}
int jj_critic_on(void *lk)
{
	return sem_wait((sem_t *)lk);
}
int jj_critic_off(void *lk)
{
	return sem_post((sem_t *)lk);
}
