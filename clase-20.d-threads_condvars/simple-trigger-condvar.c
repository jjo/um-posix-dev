#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

int flag;		/* EL flag */
pthread_cond_t flag_cv; /* condition var asociada */
pthread_mutex_t flag_mutex; /* mutex asociado */

void *func(void * arg);
#define NTHREAD_MAX 20
void msleep( int msec );
int procesa_args(int argc, char *argv[]);
int n_hilos;
int msec_disparo;
int msec_consumo;
int main(int argc, char *argv[])
{
	pthread_t tid[NTHREAD_MAX];
	int i;

	if (!procesa_args(argc, argv))
		return 1;

	flag=0;
	pthread_mutex_init(&flag_mutex, NULL);
	pthread_cond_init(&flag_cv, NULL);

	for(i=0;i<n_hilos;i++)
		pthread_create(&tid[i], NULL, func, (void*) 'A'+i );

	i=10;
	printf("lanzando 10 disparos , %d hilos ...\n", n_hilos);
	/* bucle principal del disparador */
	while(i--) {
		msleep(msec_disparo);
		pthread_mutex_lock(&flag_mutex);
		flag=1;
/*%%*/		pthread_cond_signal(&flag_cv);
		pthread_mutex_unlock(&flag_mutex);
	}
	/* se~alizacion de fin: ... */
	pthread_mutex_lock(&flag_mutex);
	flag=-1;
/*%%*/	pthread_cond_broadcast(&flag_cv);
	pthread_mutex_unlock(&flag_mutex);

	/* espera de fin de hilos */
	for(i=0;i<n_hilos;i++)
		pthread_join(tid[i], NULL);
	write(STDOUT_FILENO, "\n", 1);
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
/*%%*/			pthread_cond_wait(&flag_cv, &flag_mutex);
		}
		if (flag<0) cont=0; /* se~al de fin ... */
		else flag=0;

		pthread_mutex_unlock(&flag_mutex);
		if(cont)write(STDOUT_FILENO, &ch, 1);
		msleep(msec_consumo);
	}
	write(STDOUT_FILENO, "#", 1);
	return NULL;
}

int procesa_args(int argc, char *argv[])
{
	if (argc!=4) {
		fprintf(stderr, "uso: %s n_hiles msec_disparo msec_consumo\n"
				" ej: %s 1 50 10\n"
				" ej: %s 5 50 10\n"
				" ej: %s 5 10 50 (pierde ...)\n"
				, argv[0], argv[0], argv[0], argv[0]);
		return 0;
	}
	n_hilos=atoi(argv[1]);
	msec_disparo=atoi(argv[2]);
	msec_consumo=atoi(argv[3]);
	if (!n_hilos|| n_hilos > NTHREAD_MAX || !msec_disparo || !msec_consumo) return 0;
	return 1;
}
void msleep( int msec ) {
	struct timespec ts = { 0, msec * 1E6 };
	nanosleep(&ts, NULL);
}
