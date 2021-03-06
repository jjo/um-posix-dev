%{
#include <stdio.h>
#include <string.h>
int yyerror(char *str);
int yylex(void);
#define YYERROR_VERBOSE
extern void lex_state_esp(void);
extern void lex_state_ike(void);
%}
%union {
	int id;
	int num;
}
%start list
%token <num> tNUMBER
%token <id>  tENC
%token <id> tAUTH
%token <id> tMODP
%left	','
%left	'-'
%left   HERE
%%
list:
	|	esp_string
	|	esp_string ',' list
;
esp_string:	enc_alg  
			{putchar('\n');}
	|	enc_alg '-' dh
			{putchar('\n');}
	| 	enc_alg '-' auth_alg
			{putchar('\n');}
	|	enc_alg '-' auth_alg '-' dh
			{putchar('\n');}
	| 	enc_alg '-' auth_alg '-' tNUMBER
			{putchar('\n');}
	|	enc_alg '-' auth_alg '-' tNUMBER '-' dh
			{putchar('\n');}
;
enc_alg:	tENC	
       			{ printf ("enc_alg=%d", $1); } 
	|	tENC tNUMBER 
			{ printf ("enc_alg=%d(%d)", $1, $2); }
;
auth_alg:	tAUTH   
			{ printf (",auth_alg=%d", $1); }
;
dh:		tMODP tNUMBER  
  			{ printf (",modp=%d", $2);}
%%
int main(void)
{
	memset(&yylval, 0, sizeof(yylval));
	lex_state_esp();
	yyparse();
	return 0;
}

int yyerror(char *str)
{
	extern const char *yytext;
	fprintf(stderr, "%s  '%s'\n", str, yytext);
	return 0;
}
