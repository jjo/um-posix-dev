/* 
 * $Id: libjj_mutex.c,v 1.3 2003/10/30 22:01:48 jjo Exp $
 *
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 * License: GPLv2
 *
 * Implementacion de region critica con pthread_mutex_t:
 * 
 * Es la mas "natural" para POSIX, dado que los mutex
 * esta'n dise~ados para ello.
 *
 * No necesariamente la ma's veloz, suele implementarse
 * con spinlocks
 *
 */
#include <stdlib.h>
#include <pthread.h>
#include "libjj.h"

void *jj_critic_new(void)
{
/*%%*/	pthread_mutex_t *lk=malloc(sizeof *lk);
	if (!lk) return NULL;
/*%%*/	pthread_mutex_init(lk, NULL);
	return lk;
}
int jj_critic_on(void *lk)
{
/*%%*/	return pthread_mutex_lock((pthread_mutex_t*)lk);
}
int jj_critic_off(void *lk)
{
/*%%*/	return pthread_mutex_unlock((pthread_mutex_t*)lk);
}
