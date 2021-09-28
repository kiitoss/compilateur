/*
Source code: https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/genprogc/ie_prog_4lex_yacc.htm
*/

%{
	#include <stdio.h>
	int regs[26];
	int base;
%}

%start list
%token DIGIT LETTER
%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS  /*supplies precedence for unary minus */

%%

list:                       /*empty */
	| list stat '\n'
      	| list error '\n'       { yyerrok; }
      	;
stat:   expr                    { printf("%d\n",$1); }
        | LETTER '=' expr       { regs[$1] = $3; }
        ;
expr:   '(' expr ')'            { $$ = $2; }
        | expr '*' expr         { $$ = $1 * $3; }
        | expr '/' expr         { $$ = $1 / $3; }
        | expr '%' expr         { $$ = $1 % $3; }
        | expr '+' expr         { $$ = $1 + $3; }
        | expr '-' expr         { $$ = $1 - $3; }
        | expr '&' expr         { $$ = $1 & $3; }
        | expr '|' expr         { $$ = $1 | $3; }
        | '-' expr %prec UMINUS { $$ = -$2; }
        | LETTER                { $$ = regs[$1]; }
        | number
        ;
number: DIGIT                   { $$ = $1; base = ($1==0) ? 8 : 10; }
        | number DIGIT          { $$ = base * $1 + $2; }
        ;

%%

int main(int argc, char *argv[]) {
	return(yyparse());
}

char *s;
void yyerror(s) {
	fprintf(stderr, "%s\n",s);
}

int yywrap() {
	return(1);
}
