#include <stdlib.h>
#include <semaphore.h>
#include "locklib.h"

void *locklib_new(void)
{
	sem_t *lk=malloc(sizeof (sem_t));
	if (!lk) return NULL;
	sem_init(lk, 0, 1);
	return lk;
}
int locklib_lock(void *lk)
{
	return sem_wait((sem_t *)lk);
}
int locklib_unlock(void *lk)
{
	return sem_post((sem_t *)lk);
}
