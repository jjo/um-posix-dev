/* libcifrado.c */
#include <openssl/evp.h>
typedef EVP_CIPHER_CTX libcifrado_t;
EVP_CIPHER_CTX *libcifrado_new(void);
int libcifrado_free(EVP_CIPHER_CTX *ctx_p);
int libcifrado_set_clave(EVP_CIPHER_CTX *ctx_p, const char *clave, int cifrar);
int libcifrado_bloque(EVP_CIPHER_CTX *ctx_p, int8_t *outbuf, int *olen, int8_t *inbuf, int ilen);
int libcifrado_bloque_final(EVP_CIPHER_CTX *ctx_p, int8_t *outbuf, int *olen);
