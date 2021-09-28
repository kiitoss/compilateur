%{
	#include <stdio.h>
	#include "y.tab.h"
	int c;
	extern int yylval;
%}

%%

" " 			;
[a-z]   		{
					c = yytext[0];
					yylval = c - 'a';
					return(LETTER);
				}
