#include <glib.h>
#include <pthread.h>
#include <semaphore.h>

struct mensaje {
	int num;
};

/* NOTAR que _NO_ esta definido el tipo struct lista ... es simplemente un puntero */
struct lista *lista_new();
void lista_destroy(struct lista *lista);
void lista_put(struct lista *lista, struct mensaje *m);
/** debe DORMIR hasta devolver un elem */
struct mensaje *lista_get(struct lista *lista); 
/** inicializa un mensaje (debe estar creado previamente) */
void mensaje_init(struct mensaje *m, int num);
