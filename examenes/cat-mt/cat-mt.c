#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "buflib.h"

struct mibuf_t mibuffer;
void * lector(void *arg)
{
	char buffer[4096];
	int nread;
	while((nread=buf_read(&mibuffer, buffer, sizeof buffer))>=0) {
		printf("READ\n");
		write(1, buffer, nread);
	}
		printf("READ FIN\n");
	return NULL;
}
void * escritor(void *arg)
{
	char buffer[4096];
	int nread;
	while((nread=read(0, buffer, sizeof buffer))>=0) {
		buf_write(&mibuffer, buffer, nread);
		printf("WRITE\n");
		if(nread==0) break;
	}
	buf_close(&mibuffer);

		printf("WRITE FIN\n");
	return NULL;
}
pthread_t hilos[2];
int main(void)
{
	buf_init(&mibuffer);
	if (pthread_create(&hilos[0], NULL, lector, NULL))
			perror("pthread_create()");
	if (pthread_create(&hilos[1], NULL, escritor, NULL))
			perror("pthread_create()");
	pthread_join(hilos[0], NULL);
	pthread_join(hilos[1], NULL);
	return 0;
}
