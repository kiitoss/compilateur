%{
    #include "y.tab.h"
    #include <stdlib.h>
		
    void yyerror(char *);

		// Définition de la constante permettant de pointer la ligne de l'erreur
		int line = 1;
%}

digit				[0-9]
letter			[a-zA-Z]

%%

	/* On ignore les espaces */
[ \t]							;

	/* Déclaration des tokens */
"DEBUT"						return DEBUT;
"FIN"							return FIN;
";"								return POINT_VIRGULE;
":"								return DEUX_POINTS;
"print"						return AFFICHE;
"AFFICHE"					return AFFICHE;
"PROG"						return PROG;
"var"							return VARIABLE;

	/* Permet d'incrémenter la ligne de l'emplacement de l'erreur */
\n								{ line++; }

	/* Déclaration des nombres */
{digit}+    			{
											yylval = atoi(yytext);
											return ENTIER;
									}

	/* Déclaration des symboles usuels */
[-()<>=+*/;{}.]		return *yytext;

	/* A modifier */
{letter}					return IDF;

	/* Cas d'erreur */
.           			yyerror("Caractère invalide !");

%%

int yywrap(void) {
    return 1;
}