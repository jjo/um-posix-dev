#include <semaphore.h>
#include <string.h>

#include "buflib.h"
void buf_init (struct mibuf_t *mibuf)
{
	memset(mibuf, 0, sizeof(struct mibuf_t));
	sem_init(&mibuf->listo_pa_escribir, 0, 1);
	sem_init(&mibuf->listo_pa_leer, 0, 0);
	mibuf->len=0;
}
ssize_t buf_read (struct mibuf_t *mibuf, unsigned char *b, ssize_t len)
{
	sem_wait(&mibuf->listo_pa_leer);
	if (mibuf->len < len)	/* truncar */
		len=mibuf->len;
	memcpy(b, mibuf->data, len);
	mibuf->len=0;
	sem_post(&mibuf->listo_pa_escribir);
	return len;
}
ssize_t buf_write (struct mibuf_t *mibuf, const unsigned char *b, ssize_t len)
{
	sem_wait(&mibuf->listo_pa_escribir);
	memcpy(mibuf->data, b, len);
	mibuf->len=len;
	sem_post(&mibuf->listo_pa_leer);
	return len;
}
void buf_close(struct mibuf_t *mibuf)
{
	sem_wait(&mibuf->listo_pa_escribir);
	mibuf->len=-1;
	sem_post(&mibuf->listo_pa_leer);
}
