#include <stdio.h>
#include <unistd.h>
#include <openssl/evp.h>

#include "libcifrado.h"
EVP_CIPHER_CTX *libcifrado_new()
{
	int ret;
	EVP_CIPHER_CTX *ctx_p=malloc(sizeof *ctx_p);
	if (!ctx_p) return NULL;
	EVP_CIPHER_CTX_init(ctx_p);
	ret=EVP_CipherInit_ex(ctx_p, EVP_aes_128_ecb(), NULL, NULL, NULL, -1);
	if (!ret) {
		fprintf(stderr, "ERROR: EVP_CipherInit_ex()=%d\n", ret);
		libcifrado_free(ctx_p); ctx_p=NULL;
	}
	return ctx_p;
}
int libcifrado_set_clave(EVP_CIPHER_CTX *ctx_p, const char *clave, int cifrar /*1: cifra, 0: descifra */)
{
	int ret=EVP_CipherInit_ex(ctx_p, NULL, NULL, (u_int8_t *)clave, NULL, cifrar);
	if (!ret) {
		fprintf(stderr, "ERROR: EVP_CipherInit_ex(..., clave,cifrar)=%d\n", ret);
	}
	return ret;
}
int libcifrado_free(EVP_CIPHER_CTX *ctx_p)
{
	int ret=EVP_CIPHER_CTX_cleanup(ctx_p);
	free(ctx_p);
	return ret;
}
int libcifrado_bloque_update(EVP_CIPHER_CTX* ctx_p, int8_t *outbuf, int *olen, int8_t *inbuf, int ilen)
{
	int ret=EVP_CipherUpdate(ctx_p, (u_int8_t*) outbuf, olen, (u_int8_t*) inbuf, ilen);
	//if(!ret) fprintf(stderr, "ERROR: EVP_CipherUpdate olen=%d, ilen=%d\n", *olen, ilen);
	return ret;
}
int libcifrado_bloque_final(EVP_CIPHER_CTX* ctx_p, int8_t *outbuf, int *olen)
{
	int ret=EVP_CipherFinal(ctx_p, (u_int8_t*) outbuf, olen);
	//if (!ret) fprintf(stderr, "ERROR: EVP_CipherFinal olen=%d\n", *olen);
	return ret;
}
int libcifrado_bloque(EVP_CIPHER_CTX* ctx_p, int8_t *buf, int buflen)
{
	char local_buf[4096];
	int olen;
	int ret;
	int n;
	ret=libcifrado_bloque_update(ctx_p, local_buf, &olen, buf, buflen);
	if (!ret) return ret;
	n=olen;
	memcpy(buf, local_buf, olen);
	ret=libcifrado_bloque_final(ctx_p, local_buf, &olen);
	if (!ret) return ret;
	n+=olen;
	memcpy(buf, local_buf, olen);
	return olen;
}
