/*
 * $Id: lista-sem.c,v 1.3 2004/10/15 19:50:25 jjo Exp $
 * Autor: JuanJo Ciarlante <jjo@um.edu.ar>
 * Licencia: GPLv2
 *
 * Lista (cola) sincronizada por semaforo (si lista vacía)
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "lista.h"
#include <assert.h>

struct lista {
	GQueue *l_queue;	/* cola de elementos */
	pthread_mutex_t l_lock;	/* protege modif de la lista */
	sem_t l_sem;		/* lleva la "cuenta" de cant. de elementos */
};
void mensaje_init(struct mensaje *m) 
{
	m->num=0;
}
struct lista *lista_new(int elem_max)
{
	struct lista *l=malloc(sizeof *l);
	if (elem_max)
		fprintf(stderr, "WARNING: elem_max NO implementado para en " __FILE__ "\n");
	pthread_mutex_init(&l->l_lock, NULL);
	sem_init(&l->l_sem, 0, 0);
	l->l_queue=g_queue_new();
	return l;
}
void lista_destroy(struct lista *lista)
{
	sem_destroy(&lista->l_sem);
	g_queue_free(lista->l_queue);
}
void lista_put(struct lista *lista, struct mensaje *m)
{
	pthread_mutex_lock(&lista->l_lock);
	g_queue_push_tail(lista->l_queue, m);	/* agrego elem a la cola de la lista */
	pthread_mutex_unlock(&lista->l_lock);
	sem_post(&lista->l_sem);		/* semaforo: UP */
}
struct mensaje *lista_get(struct lista *lista)
{
	struct mensaje *m;
	sem_wait(&lista->l_sem);		/* semaforo: DOWN */
	pthread_mutex_lock(&lista->l_lock);
	m=g_queue_pop_head(lista->l_queue);
	pthread_mutex_unlock(&lista->l_lock);
	assert(m!=NULL);
	return m;	
}

