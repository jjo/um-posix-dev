#include <semaphore.h>
struct mibuf_t {
	sem_t listo_pa_escribir;
	sem_t listo_pa_leer;
	int len;
	unsigned char *data[4096];
};
/* buflib.c */
void buf_init(struct mibuf_t *buffer);
size_t buf_read(struct mibuf_t *buffer, unsigned char *b, size_t len);
size_t buf_write(struct mibuf_t *buffer, unsigned char *b, size_t len);
void buf_close(struct mibuf_t *mibuffer);
