#include <glib.h>
#include <pthread.h>
#include <semaphore.h>
/* lista 2blemente linkeada */
struct lista {
	GQueue *l_queue;	/* cola de elementos */
	pthread_mutex_t l_lock;	/* protege modif de la lista */
	union {
		sem_t l_sem;		/* lleva la "cuenta" de cant. de elementos */
		pthread_cond_t l_cond;
	} u;
};
struct mensaje {
	int num;
};


/* lista.c */
void lista_init(struct lista *lista);
void lista_destroy(struct lista *lista);
void lista_put(struct lista *lista, struct mensaje *m);
/** debe DORMIR hasta devolver un elem */
struct mensaje *lista_get(struct lista *lista); 
/** inicializa un mensaje (debe estar creado previamente) */
void mensaje_init(struct mensaje *m);
