%{
#include <stdlib.h>
#include "algparse-parse.tab.h"
%}
%{
#define ip_address u_int32_t
#define ip_subnet u_int32_t
#include "/home/jjo/src/juanjo/freeswan/pluto/constants.h"
#define ESP_SERPENT 252
#define ESP_TWOFISH 253
/*"aes"	{ return ESP_AES; }*/
%}

%%
"aes"		{ yylval.id=ESP_AES; 	return tENC; }
"blowfish" 	{ yylval.id=ESP_BLOWFISH; 	return tENC; }
"cast" 		{ yylval.id=ESP_CAST;	return tENC; }
"twofish" 	{ yylval.id=ESP_TWOFISH;	return tENC; }
"serpent"	{ yylval.id=ESP_SERPENT;	return tENC; } 
"3des"		{ yylval.id=ESP_3DES;	return tENC; }
"md5"		{ yylval.id=AUTH_ALGORITHM_HMAC_MD5; 	return tAUTH; }
"ripemd"	{ yylval.id=AUTH_ALGORITHM_HMAC_RIPEMD;	return tAUTH; }
"sha1"		{ yylval.id=AUTH_ALGORITHM_HMAC_SHA1;	return tAUTH; }
"sha2_256"	{ yylval.id=AUTH_ALGORITHM_HMAC_SHA2_256;	return tAUTH; }
"sha2_512"	{ yylval.id=AUTH_ALGORITHM_HMAC_SHA2_512;	return tAUTH; }
"modp"		{ return tMODP; }
[0-9]+		{ yylval.num=atoi(yytext);return tNUMBER;}
.		{ return yytext[0]; }
%%
void dummy(void){
	yyunput(1,"a");
}
