#include <stdio.h>
#include <openssl/asn1.h>
#include <openssl/dh.h>
#include <openssl/err.h>

int main(void)
{
	DH *dh;
	FILE *f=fopen("dhparam.dat", "w+");
	if (!f) return 255;
	if (!(dh=DH_generate_parameters(512, 2, NULL, NULL))) {
		ERR_print_errors_fp(stderr);
		return 1;
	}
	if (i2d_DHparams_fp(f, dh)!=0) {
		ERR_print_errors_fp(stderr);
		return 2;
	}
	return 0;
}
