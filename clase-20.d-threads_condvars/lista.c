#include "linux_list.h"
#include "lista.h"
void mensaje_init(struct mensaje *m) 
{
	INIT_LIST_HEAD(&m->l_ptr);
	m->num=0;
}
void lista_init(struct lista *lista)
{
	pthread_mutex_init(&lista->l_lock, NULL);
	sem_init(&lista->l_sem, 0, 0);
	INIT_LIST_HEAD(&lista->l_head);
}
void lista_destroy(struct lista *lista)
{
	sem_destroy(&lista->l_sem);
}
void lista_put(struct lista *lista, struct mensaje *m)
{
	pthread_mutex_lock(&lista->l_lock);
	list_add_tail(&m->l_ptr, &lista->l_head);
	pthread_mutex_unlock(&lista->l_lock);
	sem_post(&lista->l_sem);
}
struct mensaje *lista_get(struct lista *lista)
{
	struct mensaje *m;
	sem_wait(&lista->l_sem);
	pthread_mutex_lock(&lista->l_lock);
	m=list_entry(lista->l_head.next, struct mensaje, l_ptr);
	list_del(&m->l_ptr);
	pthread_mutex_unlock(&lista->l_lock);
	return m;	
}

