#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <pthread.h>
#include "locklib.h"

void *locklib_new(void)
{
	pthread_rwlock_t *lk=malloc(sizeof (pthread_rwlock_t));
	if (!lk) return NULL;
	pthread_rwlock_init(lk, NULL);
	return lk;
}
int locklib_lock(void *lk)
{
	return pthread_rwlock_wrlock((pthread_rwlock_t*)lk);
}
int locklib_unlock(void *lk)
{
	return pthread_rwlock_unlock((pthread_rwlock_t*)lk);
}
