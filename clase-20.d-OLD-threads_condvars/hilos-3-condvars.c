#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int x=0;

void *h2(void *data)
{
	while(1) {
		pthread_mutex_lock(&mut);
		x++;
		pthread_mutex_unlock(&mut);
		pthread_cond_signal(&cond);
		printf("x=%d\n", x);
		usleep(1000000/5);
	}
	return NULL;
}

void *h1(void *data)
{
	pthread_mutex_lock(&mut);
	while (x <= 10) {
		printf(".\n");
		pthread_cond_wait(&cond, &mut);
	}
	pthread_mutex_unlock(&mut);
	return NULL;
}
int main(void) {
	pthread_t tid[2];
	pthread_create ( &tid[0], NULL, h1, NULL);
	pthread_create ( &tid[1], NULL, h2, NULL);
	pthread_join ( tid[0], NULL);
	return 0;
}
