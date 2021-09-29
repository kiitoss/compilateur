%{
	#include <stdio.h>
	#include "y.tab.h"
	int c;
	extern int yylval;
%}

%%

" " 			;

"<="			{ return INFOUEG; }
">="			{ return SUPOUEG; }
"not"			{ return NON; }
"and"			{ return ET; }
"ou"			{ return OU; }
[-+*/=<>] { return yytext[0]; }

[1-9][0-9]* {
	yylval = atoi(yytext);
	printf("%d\n", atoi(yytext));
	return ENTIER;
	}

[a-z] {
	c = yytext[0];
	yylval = c - 'a';
	return LETTER;
	}
