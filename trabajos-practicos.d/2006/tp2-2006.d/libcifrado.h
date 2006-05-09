/* libcifrado.c */
#include <openssl/evp.h>
typedef EVP_CIPHER_CTX libcifrado_t;
EVP_CIPHER_CTX *libcifrado_new(void);
int libcifrado_free(EVP_CIPHER_CTX *ctx_p);
int libcifrado_set_clave(EVP_CIPHER_CTX *ctx_p, const char *clave, int largo_clave, int cifrar /*1: cifra, 0: descifra */);
int libcifrado_bloque(EVP_CIPHER_CTX *ctx_p, char *buf, int buflen);
int libcifrado_bloque_update(EVP_CIPHER_CTX *ctx_p, char *outbuf, int *olen, char *inbuf, int ilen);
int libcifrado_bloque_final(EVP_CIPHER_CTX *ctx_p, char *outbuf, int *olen);
