#include <stdio.h>
#include <openssl/asn1.h>
#include <openssl/dh.h>
#include <openssl/err.h>

int main(void)
{
	DH *dh;
	FILE *f=fopen("dhparam.dat", "r");
	if (!f) { perror("dhparam.dat");return 255;}
	dh=DH_new();
	if (!d2i_DHparams_fp(f, &dh)) {
		ERR_print_errors_fp(stderr);
		return 1;
	}
	if(DH_generate_key(dh)==0) {
		ERR_print_errors_fp(stderr);
		return 2;
	}
	return 0;
}

