/* 
 * Autor: JuanJo Ciarlante
 * Licencia: GPLv2  (OJO: ESTO ES IMPORTANTE!)
 *
 * compilar con 
 *   make pthread-condvar-espera-x-varios LDLIBS=-lpthread
 *
 * Usa "condition variables", un mecanismo de se~alizacio'n generalizado
 * que contiene fundamentalmente _wait() [dormir] y _signal() [despertar].
 * 
 * Logica del programa: main() lanza 10 threads  (cada uno incrementa en 1 
 * la variable sincronizada) y luego espera que la variable valga 10).
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_main(void *arg); /* main() de cada thread a crear */
struct sync_var {		/* contiene un entero "sincronizado" por una cond_t y un mutex_t */
	pthread_mutex_t mutex;
	pthread_cond_t  waitq;
	int num, num2wait;	/* dos enteros: num es el contador, num2wait es el nro a esperar */
};

/* inicializa la estructura: mutex, condvar y enteros */
static void
sync_var_init(struct sync_var *v, int num, int num2wait) {
	pthread_mutex_init(&v->mutex, NULL);
	pthread_cond_init(&v->waitq, NULL);
	v->num=num;
	v->num2wait=num2wait;
}

/* duerme esperando que num==num2wait */
static int
sync_var_wait4(struct sync_var *v) {
/*LOCK*/	pthread_mutex_lock(&v->mutex);
/*TEST*/	while(v->num!=v->num2wait) 
/*WAIT*/		pthread_cond_wait(&v->waitq, &v->mutex);
		v->num=0;
/*UNLOCK*/	pthread_mutex_unlock(&v->mutex);
		return v->num;
}

/* hace num+=delta y se~aliza si num==num2wait */
static int
sync_var_delta(struct sync_var *v, int delta) {
	int num;
/*LOCK*/	pthread_mutex_lock(&v->mutex);
/*MODIFY*/	v->num+=delta;
		num=v->num;
/*SIGNAL*/	if (v->num==v->num2wait) pthread_cond_signal(&v->waitq);
/*UNLOCK*/	pthread_mutex_unlock(&v->mutex);
		return num;
}
struct sync_var sv; /* global :P */

/* un simple ejemplo de uso ... */
int main(void) {
	int n=10;
	pthread_t tid;
/***/	sync_var_init(&sv, 0, 10);	/* inicializo con 0 , para esperar por 10 */
	while(n--) {
		pthread_create(&tid, NULL, thread_main, (void*)n); /*lanzo los threads */
	}
/***/	sync_var_wait4(&sv);	/* espero ... */
	printf("main saliendo...\n");
	return 0;
}

/* main de c/thread: duerme una cant random de uSecs <10000 y luego 
 * incrementa en 1 num 
 */
void *thread_main(void *arg) {
	int n=(int)arg;
	pthread_detach(pthread_self());	/* me detacheo para ahorrar recursos de "espera por mi terminacio'n" */
	usleep(random()%10000);		/* duermo una cantidad random de uSecs < 10000 */
	printf("thread_num=%d\n", n);	/* OJO: por sencillez uso printf pero no deberi'a */
/***/	sync_var_delta(&sv, +1);
	return NULL;
}
