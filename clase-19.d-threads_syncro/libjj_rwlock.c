/* 
 * $Id: libjj_rwlock.c,v 1.3 2003/10/30 22:01:48 jjo Exp $
 *
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 * License: GPLv2
 *
 * Implementacion de region critica con sem_t (semaforos):
 * 
 * Se utiliza un sema'foro "binario", inicializado en 1;
 *
 * NO es el propo'sito ti'pico de un sema'foro (orientado
 * semaforizar a "cantidad" de recursos).
 *
 * No necesariamente la ma's veloz.
 */

/*%%*/
#define _XOPEN_SOURCE 500	/* necesario para que los headers hagan
				   visible rwlock_*... */

#include <stdlib.h>
#include <pthread.h>
#include "libjj.h"

void *jj_critic_new(void)
{
/*%%*/	pthread_rwlock_t *lk=malloc(sizeof *lk);
	if (!lk) return NULL;
/*%%*/	pthread_rwlock_init(lk, NULL);
	return lk;
}
int jj_critic_on(void *lk)
{
/*%%*/	return pthread_rwlock_wrlock((pthread_rwlock_t*)lk);
}
int jj_critic_off(void *lk)
{
/*%%*/	return pthread_rwlock_unlock((pthread_rwlock_t*)lk);
}
