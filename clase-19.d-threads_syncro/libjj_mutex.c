#include <stdlib.h>
#include <pthread.h>
#include "locklib.h"

void *locklib_new(void)
{
	pthread_mutex_t *lk=malloc(sizeof *lk);
	if (!lk) return NULL;
	pthread_mutex_init(lk, NULL);
	return lk;
}
int locklib_lock(void *lk)
{
	return pthread_mutex_lock((pthread_mutex_t*)lk);
}
int locklib_unlock(void *lk)
{
	return pthread_mutex_unlock((pthread_mutex_t*)lk);
}
