#include <semaphore.h>
#include <string.h>

#include "buflib.h"
void buf_init (struct mibuf_t *mibuffer)
{
	memset(mibuffer, 0, sizeof(struct mibuf_t));
	sem_init(&mibuffer->listo_pa_escribir, 0, 1);
	sem_init(&mibuffer->listo_pa_leer, 0, 0);
	mibuffer->len=0;
}
size_t buf_read (struct mibuf_t *mibuffer, unsigned char *b, size_t len)
{
	sem_wait(&mibuffer->listo_pa_leer);
	if (mibuffer->len < len) len=mibuffer->len;	/* truncar */
	memcpy(b, mibuffer->data, len);
	sem_post(&mibuffer->listo_pa_escribir);
	return len;
}
size_t buf_write (struct mibuf_t *mibuffer, unsigned char *b, size_t len)
{
	sem_wait(&mibuffer->listo_pa_escribir);
	memcpy(mibuffer->data, b, len);
	mibuffer->len=len;
	sem_post(&mibuffer->listo_pa_leer);
	return len;
}
void buf_close(struct mibuf_t *mibuffer)
{
	mibuffer->len=-1;
	sem_post(&mibuffer->listo_pa_leer);
}
