#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

int flag;
pthread_cond_t flag_cv;
pthread_mutex_t flag_mutex;
void *func(void * arg);
void msleep( int msec ) {
	struct timespec ts = { 0, msec * 1E6 };
	nanosleep(&ts, NULL);
}
#define NTH 5
int main(void)
{
	pthread_t tid[5];
	int i;

	pthread_mutex_init(&flag_mutex, NULL);
	pthread_cond_init(&flag_cv, NULL);
	for(i=0;i<NTH;i++)
		pthread_create(&tid[i], NULL, func, (void*) 'A'+i );

	i=10;
	while(i--) {
		msleep(100);
		pthread_mutex_lock(&flag_mutex);
		flag++;
		flag++;
		pthread_cond_signal(&flag_cv);
		pthread_mutex_unlock(&flag_mutex);
	}
	pthread_mutex_lock(&flag_mutex);
	flag=-1;
	pthread_cond_broadcast(&flag_cv);
	pthread_mutex_unlock(&flag_mutex);
	for(i=0;i<NTH;i++)
		pthread_join(tid[i], NULL);
	return 0;
}
void *func(void * arg)
{
	int ch=(int) arg;
	int cont=1;
	while(cont) {
		pthread_mutex_lock(&flag_mutex);
		while(flag==0) {
			write(STDOUT_FILENO, ".", 1);
			pthread_cond_wait(&flag_cv, &flag_mutex);
		}
		if (flag>0) flag--;
		if (flag<0) cont=0;
		pthread_mutex_unlock(&flag_mutex);
		write(STDOUT_FILENO, &ch, 1);
		msleep(50);
	}
	write(STDOUT_FILENO, "X", 1);
	return NULL;
}

