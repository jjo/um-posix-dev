#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "buflib.h"

struct mibuf_t mibuf;
void * lector(void *arg)
{
	char buffer[4096];
	int nbytes;
	while((nbytes=buf_read(&mibuf, buffer, sizeof buffer))>=0) {
		/* fprintf(stderr, "READ %d\n", nbytes); */
		write(STDOUT_FILENO, buffer, nbytes);
		if(nbytes==0) break;
	}
/* 	fprintf(stderr, "READ FIN\n"); */
	return NULL;
}
void * escritor(void *arg)
{
	char buffer[4096];
	int nbytes;
	while((nbytes=read(STDIN_FILENO, buffer, sizeof buffer))>=0) {
		buf_write(&mibuf, buffer, nbytes);
/* 		fprintf(stderr, "WRITE %d\n", nbytes); */
		if(nbytes==0) break;
	}
/* 	buf_close(&mibuf); */
/* 	fprintf(stderr, "WRITE FIN\n"); */
	return NULL;
}
pthread_t hilos[2];
int main(void)
{
	buf_init(&mibuf);
	if (pthread_create(&hilos[0], NULL, lector, NULL))
			perror("pthread_create()");
	if (pthread_create(&hilos[1], NULL, escritor, NULL))
			perror("pthread_create()");
	pthread_join(hilos[0], NULL);
	pthread_join(hilos[1], NULL);
	return 0;
}
