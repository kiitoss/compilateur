%{
	#include <stdio.h>
	#include "y.tab.h"
	int c;
	extern int yylval;
%}

letter								[a-zA-Z]
digit									[0-9]			

%%

" "										;

"<="									{ return INFOUEG; }
">="									{ return SUPOUEG; }
("not"|"!")						{ return NON; }
("and"|"&&")					{ return ET; }
("or"|"||")						{ return OU; }

[-+*/=<>]							{ return yytext[0]; }

{digit}+{digit}*			{
	yylval = atoi(yytext);
	return ENTIER;
}

{digit}+(.|,)+{digit}*	{
	yylval = atof(yytext);
	return REEL;
}

{letter}							{
	c = yytext[0];
	yylval = c - 'a';
	return LETTER;
}

{letter}+							{
	printf("%s\n", yytext);
}
