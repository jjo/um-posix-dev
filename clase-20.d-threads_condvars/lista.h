#include <pthread.h>
#include <semaphore.h>
#include "linux_list.h"
/* lista 2blemente linkeada */
struct lista {
	struct list_head l_head;
	pthread_mutex_t l_lock;
	union {
		sem_t l_sem;
		pthread_cond_t l_cond;
	} u;
};
/* cada mensaje va a ser 1elem. de la lista */
struct mensaje {
	int num;
	struct list_head l_ptr;
};


/* lista.c */
void lista_init(struct lista *lista);
void lista_destroy(struct lista *lista);
void lista_put(struct lista *lista, struct mensaje *m);
struct mensaje *lista_get(struct lista *lista);
void mensaje_init(struct mensaje *m);
