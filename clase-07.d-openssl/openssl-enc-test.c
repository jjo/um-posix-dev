/* $Id: openssl-enc-test.c,v 1.1 2003/05/02 20:53:57 jjo Exp $ */
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

unsigned char KEY[] ={ "012345678abdcef" };
unsigned char IV[]= { "01234567" };
#define BLOCK_SIZE 1024

int main(int argc, char * const argv[])
{
	char inbuf[BLOCK_SIZE+EVP_MAX_IV_LENGTH];
	char outbuf[BLOCK_SIZE+EVP_MAX_IV_LENGTH];
	int ilen, olen;
	int do_enc;
/*1*/	EVP_CIPHER_CTX ctx;
	if (argc!=2) {
		fprintf(stderr, "uso: %s {0|1}\n", argv[0]);
		return 255;
	}
	/* do_enc: 0=decifra 1=cifra */
	do_enc=atoi(argv[1]);

	/* inicializa el "contexto" del cifrador */
/*2*/	EVP_CIPHER_CTX_init(&ctx);
	/* asocia un algoritmo:
	 * bf_cbc:	BLOWFISH (block), 128 bits
	 * cast5_cbc:	CAST (block), 128 bits
	 * rc4:		RC4 (stream), 128 bits
	 */
/*3a*/	EVP_CipherInit(&ctx, EVP_bf_cbc(), NULL, NULL, 0);
	
	/* setea una clave e initialization-vector para CBC */
/*3b*/	EVP_CipherInit(&ctx, NULL, KEY, IV, do_enc);

	while ((ilen=read (STDIN_FILENO, inbuf, BLOCK_SIZE)) > 0) {
		olen=ilen;
		/* CipherUpdate: cifra desde inbuf[ilen-1] a outbuf[olen-1] */
/*4*/		if(!EVP_CipherUpdate(&ctx, outbuf, &olen, inbuf, ilen)) {
			fprintf(stderr, "EVP_CipherUpdate error\n");
		}
		write(STDOUT_FILENO, outbuf, olen);
	}
	/* CipherFinal: último cifrado (necesario para CBC) */
/*5*/	if (!EVP_CipherFinal(&ctx, outbuf, &olen)) {
		fprintf(stderr, "EVP_CipherFinal error\n");
	}
	write(STDOUT_FILENO, outbuf, olen);
/*6*/	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}
