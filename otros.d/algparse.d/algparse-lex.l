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
%}

%s esp
%s ike
%%
<esp>"aes"		{ yylval.id=ESP_AES; 	return tENC; }
<esp>"blowfish" 	{ yylval.id=ESP_BLOWFISH; 	return tENC; }
<esp>"cast" 		{ yylval.id=ESP_CAST;	return tENC; }
<esp>"twofish" 	{ yylval.id=ESP_TWOFISH;	return tENC; }
<esp>"serpent"	{ yylval.id=ESP_SERPENT;	return tENC; } 
<esp>"3des"		{ yylval.id=ESP_3DES;	return tENC; }
<esp>"md5"		{ yylval.id=AUTH_ALGORITHM_HMAC_MD5; 	return tAUTH; }
<esp>"ripemd"	{ yylval.id=AUTH_ALGORITHM_HMAC_RIPEMD;	return tAUTH; }
<esp>"sha1"		{ yylval.id=AUTH_ALGORITHM_HMAC_SHA1;	return tAUTH; }
<esp>"sha2_256"	{ yylval.id=AUTH_ALGORITHM_HMAC_SHA2_256;	return tAUTH; }
<esp>"sha2_512"	{ yylval.id=AUTH_ALGORITHM_HMAC_SHA2_512;	return tAUTH; }
<esp>"modp"		{ return tMODP; }
<ike>"aes"		{ yylval.id=OAKLEY_AES_CBC; 	return tENC; }
<ike>"blowfish" 	{ yylval.id=OAKLEY_BLOWFISH_CBC; 	return tENC; }
<ike>"cast" 		{ yylval.id=OAKLEY_CAST_CBC;	return tENC; }
<ike>"twofish" 	{ yylval.id=99999;	return tENC; }
<ike>"serpent"	{ yylval.id=99999;	return tENC; } 
<ike>"3des"		{ yylval.id=OAKLEY_3DES_CBC;	return tENC; }
<ike>"md5"		{ yylval.id=OAKLEY_MD5; 	return tAUTH; }
<ike>"ripemd"	{ yylval.id=99999;	return tAUTH; }
<ike>"sha"		{ yylval.id=OAKLEY_SHA;	return tAUTH; }
<ike>"sha2_256"	{ yylval.id=OAKLEY_SHA2_256;	return tAUTH; }
<ike>"sha2_512"	{ yylval.id=OAKLEY_SHA2_512;	return tAUTH; }
[0-9]+		{ yylval.num=atoi(yytext);return tNUMBER;}
.		{ return yytext[0]; }
%%
void lex_state_esp(void) { BEGIN(esp); }
void lex_state_ike(void) { BEGIN(ike); }
void dummy(void){
	yyunput(1,"a");
}
