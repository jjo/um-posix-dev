/*
 * $Id: lista-condvar.c,v 1.1 2004/10/22 22:24:02 jjo Exp $
 * Autor: JuanJo Ciarlante <jjo@um.edu.ar>
 * Licencia: GPLv2
 *
 * Lista (cola) sincronizada por condition variable
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include "lista.h"
#include <assert.h>

struct lista {
	GQueue *l_queue;	/* cola de elementos */
	pthread_mutex_t l_lock;	/* protege modif de la lista */
	pthread_cond_t l_cond;	/* para dormir/despertar */
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
	memset(l, 0, sizeof *l);
	pthread_mutex_init(&l->l_lock, NULL);
	pthread_cond_init(&l->l_cond, NULL);
	l->l_queue=g_queue_new();
	return l;
}
void lista_destroy(struct lista *lista)
{
	pthread_mutex_destroy(&lista->l_lock);
	pthread_cond_destroy(&lista->l_cond);
	g_queue_free(lista->l_queue);
}
void lista_put(struct lista *lista, struct mensaje *m)
{
	pthread_mutex_lock(&lista->l_lock);
	g_queue_push_tail(lista->l_queue, m);	/* agrego elem a la cola de la lista */
	pthread_mutex_unlock(&lista->l_lock);
	pthread_cond_signal(&lista->l_cond);		/* BANG! */
}
struct mensaje *lista_get(struct lista *lista)
{
	struct mensaje *m;
	pthread_mutex_lock(&lista->l_lock);
	while ((m=g_queue_pop_head(lista->l_queue))==NULL) {
		pthread_cond_wait(&lista->l_cond, &lista->l_lock);		/* zzzZZzz */
	}
	pthread_mutex_unlock(&lista->l_lock);
	assert(m!=NULL);
	return m;	
}

