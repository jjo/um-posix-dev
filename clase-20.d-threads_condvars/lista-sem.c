/*
 * $Id: lista-sem.c,v 1.2 2004/10/15 19:24:14 jjo Exp $
 * Autor: JuanJo Ciarlante <jjo@um.edu.ar>
 * Licencia: GPLv2
 *
 * Lista (cola) sincronizada por semaforo (si lista vacía)
 */

#include <glib.h>
#include "lista.h"
#include <assert.h>
void mensaje_init(struct mensaje *m) 
{
	m->num=0;
}
void lista_init(struct lista *lista)
{
	pthread_mutex_init(&lista->l_lock, NULL);
	sem_init(&lista->u.l_sem, 0, 0);
	lista->l_queue=g_queue_new();
}
void lista_destroy(struct lista *lista)
{
	sem_destroy(&lista->u.l_sem);
	g_queue_free(lista->l_queue);
}
void lista_put(struct lista *lista, struct mensaje *m)
{
	pthread_mutex_lock(&lista->l_lock);
	g_queue_push_tail(lista->l_queue, m);	/* agrego elem a la cola de la lista */
	pthread_mutex_unlock(&lista->l_lock);
	sem_post(&lista->u.l_sem);		/* semaforo: UP */
}
struct mensaje *lista_get(struct lista *lista)
{
	struct mensaje *m;
	sem_wait(&lista->u.l_sem);		/* semaforo: DOWN */
	pthread_mutex_lock(&lista->l_lock);
	m=g_queue_pop_head(lista->l_queue);
	pthread_mutex_unlock(&lista->l_lock);
	assert(m!=NULL);
	return m;	
}

