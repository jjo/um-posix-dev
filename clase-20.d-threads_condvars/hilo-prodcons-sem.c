/* 
 * producer: genera mensajes que se encolan en lalista (contienen un nro entero)
 * consumer: saca mensajes de lalista y los procesa (muestra el nro contenido)
 *
 * PESIMO programa: si descomentamos el nanosleep() no anda mas
 * ... porque?
 */
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#include "linux_list.h"
#include "lista.h"

void *consumer(void *);
void *producer(void *);
struct lista lalista;
int main(int argc, char *argv[])
{
	unsigned n_consumers, n_total;	
	pthread_t tid_cons, tid_prod;
	if (argc!=3) {
		fprintf(stderr, "uso: %s n_total n_consumers\n"
				" ej: %s 20 1\n"
				" ej: %s 20 5\n"
				, argv[0], argv[0], argv[0]);
		return 1;
	}
	n_total=atoi(argv[1]);
	n_consumers=atoi(argv[2]);
	if (!n_total|| n_total > 10000 || !n_consumers || n_consumers > 100) exit(1);
	lista_init(&lalista);

	while(n_consumers--) {
		pthread_create(&tid_cons, NULL, consumer, NULL);
	}

	pthread_create(&tid_prod, NULL, producer, (void*)n_total);
	pthread_join(tid_prod, NULL);

	lista_destroy(&lalista);
	return 0;
}
void *producer(void *arg) {
	struct mensaje *m;
	int i, n_total=(int)arg;
	for (i=0;i<n_total;i++) {
		m=malloc(sizeof *m);
		mensaje_init(m);
		m->num=i;
		lista_put(&lalista, m);
	}
	return NULL;
}
void *consumer(void *arg) {
	struct mensaje *m;
	char buf[64]; int cant;
	struct timespec ts = { 0, 0}; 

	pthread_detach(pthread_self());
	while(1) {
		m=lista_get(&lalista);
		if (!m) break;
		cant=snprintf(buf, sizeof buf -1, "consumer[%lu] num=%d\n", 
				pthread_self(),
				m->num);
		
		nanosleep(&ts, NULL);
		write(STDOUT_FILENO, buf, cant);
		free(m);
	}
	return NULL;
}
