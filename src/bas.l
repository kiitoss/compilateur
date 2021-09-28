/*
Source code: https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/genprogc/ie_prog_4lex_yacc.htm
*/

%{
	#include <stdio.h>
	#include "y.tab.h"
	int c;
	extern int yylval;
%}

%%

" "				;
[a-z]			{
					c = yytext[0];
					yylval = c - 'a';
					return(LETTER);
				}
[0-9]       	{
					c = yytext[0];
					yylval = c - '0';
					return(DIGIT);
            	}
[^a-z0-9\b] 	{
                	c = yytext[0];
                	return(c);
            	}
