/* 
 * $Id: hilo-prodcons.c,v 1.3 2004/10/15 19:50:25 jjo Exp $
 * Autor: JuanJo Ciarlante <jjo@um.edu.ar>
 * Licencia: GPLv2
 *
 * 1 producer: genera mensajes que se encolan en LaLista (contienen un nro entero)
 * n consumers: sacan mensajes de LaLista y lo procesan (muestra el nro contenido)
 *
 * el programa tiene una HORRIBLE race-condition...
 * ... porque?  `8)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#include "lista.h"
struct lista *LaLista;
void parse_args_or_die(int argc, const char *argv[], int *n_mensajes, int *n_consumers);
void *consumer(void *);
void *producer(void *);

int main(int argc, const char *argv[])
{
	unsigned n_consumers, n_mensajes, i;
	pthread_t tid_cons, tid_prod;

	parse_args_or_die(argc, argv, &n_mensajes, &n_consumers);
	LaLista=lista_new(10);

	for(i=1;i<=n_consumers;i++)
		pthread_create(&tid_cons, NULL, consumer, (void*)i);

	pthread_create(&tid_prod, NULL, producer, (void*)n_mensajes);
	pthread_join(tid_prod, NULL);

	//sleep(1); // truCHEx
	lista_destroy(LaLista);
	printf("Saliendo...\n");
	return 0;
}

void *producer(void *arg) {
	struct mensaje *m;
	int i, n_mensajes=(int)arg;
	for (i=0;i<n_mensajes;i++) {
		printf("producer: i=%d\n", i);
		m=malloc(sizeof *m);
		mensaje_init(m);
		m->num=i;
		lista_put(LaLista, m);
	}
	return NULL;
}
void *consumer(void *arg) {
	unsigned hilo_num=(unsigned)arg;
	struct mensaje *m;
	char buf[64]; int cant;

	pthread_detach(pthread_self());
	while(1) {
		usleep(10);
		m=lista_get(LaLista);
		if (!m) break;
		cant=snprintf(buf, sizeof buf -1, "                consumer[%02u] num=%d\n", hilo_num, m->num);
		
		write(STDOUT_FILENO, buf, cant);
		free(m);
	}
	return NULL;
}
void parse_args_or_die(int argc, const char *argv[], int *n_mensajes, int *n_consumers)
{
	if (argc!=3) {
		fprintf(stderr, "uso: %s n_mensajes n_consumers\n"
				" ej: %s 20 1\n"
				" ej: %s 20 5\n"
				, argv[0], argv[0], argv[0]);
		exit(255);
	}
	*n_mensajes=atoi(argv[1]);
	*n_consumers=atoi(argv[2]);
	if (!*n_mensajes|| *n_mensajes > 10000 || !*n_consumers || *n_consumers > 100) exit(255);
}
