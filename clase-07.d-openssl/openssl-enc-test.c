/* $Id: openssl-enc-test.c,v 1.3 2006/04/24 21:03:06 jjo Exp $ */
/*
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>, basado en documentacion de
 * www.openssl.org
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
#include <stdio.h>
#include <unistd.h>
#include <openssl/evp.h>

unsigned char myKEY[] ={ "0123456789abdcef" };
unsigned char IV[]= { "01234567" };	/* para CBC mode */
#define BLOCK_SIZE 1024

/*
 * Por razones didacticas uso EFB (menos seguro) que CBC
 */

int main(int argc, char * const argv[])
{
	u_int8_t inbuf[BLOCK_SIZE+EVP_MAX_IV_LENGTH];
	u_int8_t outbuf[BLOCK_SIZE+EVP_MAX_IV_LENGTH];
	int ilen, olen;
	int do_enc;
	/*
	 * Contexto para cifrado (algo asi' como una "instancia" de cifrador)
	 */
/*1*/	EVP_CIPHER_CTX ctx;

	if (argc!=2) {
		fprintf(stderr, "uso: %s {0|1}\n", argv[0]);
		return 255;
	}
	/*
	 * do_enc: 0=decifra 1=cifra
	 */
	do_enc=atoi(argv[1]);

	/*
	 * inicializa el "contexto" del cifrador
	 */

/*2*/	EVP_CIPHER_CTX_init(&ctx);

	/* asocia un algoritmo de cifrado:
	 * aes_128_ecb:	AES (block), 128 bits
	 * rc4:		RC4 (stream), 128 bits
	 */

/*3a*/	EVP_CipherInit(&ctx, EVP_aes_128_ecb(), NULL, NULL, 0);
	
	/*
	 * setea una clave (y eventualmente un initialization-vector para CBC)
	 */
/*3b*/	EVP_CipherInit(&ctx, NULL, myKEY, NULL, do_enc);

	/*
	 * loop: lee stdin->(de)cifra->escribe stdout 
	 */
	while ((ilen=read (STDIN_FILENO, inbuf, BLOCK_SIZE)) > 0) {
		olen=ilen;
		/*
		 * CipherUpdate: cifra desde inbuf[ilen-1] a outbuf[olen-1]
		 */
/*4*/		if(!EVP_CipherUpdate(&ctx, outbuf, &olen, inbuf, ilen)) {
			fprintf(stderr, "EVP_CipherUpdate error\n");
		}
		write(STDOUT_FILENO, outbuf, olen);
	}

	/*
	 * CipherFinal: u'ltimo paso de cifrado
	 */
/*5*/	if (!EVP_CipherFinal(&ctx, outbuf, &olen)) {
		fprintf(stderr, "EVP_CipherFinal error\n");
	}
	write(STDOUT_FILENO, outbuf, olen);

	/*
	 * free() del contexto (objeto) de cifrado
	 */
/*6*/	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}
