#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <security/pam_appl.h>
#include <security/pam_misc.h>

int main(void)
{
	pam_handle_t *pam_h;
	struct pam_conv pam_c;
	
	pam_c.conv = &misc_conv;
	pam_c.appdata_ptr = NULL;

	pam_start("su", getenv("USER"), &pam_c, &pam_h);

	if (pam_authenticate(pam_h, 0) != PAM_SUCCESS)
		fprintf(stderr, "Auth failed\n");
	else
		fprintf(stderr, "Auth Ok\n");

	pam_end(pam_h, 0);
	return 0;
}
