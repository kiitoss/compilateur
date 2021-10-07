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


	/* Permet d'ignorer les commentaires */
"//".*						;


	/* Permet d'ignorer les espaces */
[ \t]							;


	/* Permet d'incrémenter la ligne de l'erreur */
\n								{ line++; }


	/* Déclaration des tokens */
"{"						return DEBUT;
"}"							return FIN;
";"								return POINT_VIRGULE;
":"								return DEUX_POINTS;
"("								return PARENTHESE_OUVRANTE;
")"								return PARENTHESE_FERMANTE;
"affiche"					return AFFICHE;
"PROG"						return PROG;
"var"							return VARIABLE;
"fonction"				return FONCTION;
"retourne"				return RETOURNE;
"et"							return ET;
"ou"							return OU;
"!"								return NON;
"si"							return SI;
"alors"						return ALORS;
"sinon"						return SINON;
"vrai"						{
										yylval = 1;
										return TRUE;
									}
"faux"						{
										yylval = 0;
										return FALSE;
									}


	/* Déclaration des nombres */
[1-9]+{digit}*    {
											yylval = atoi(yytext);
											return ENTIER;
									}


	/* Déclaration des symboles usuels */
[-+*/]						return *yytext;


	/* A modifier */
{letter}					return IDF;


	/* Cas d'erreur */
.           			yyerror("Caractère invalide !");


%%

int yywrap(void) {
    return 1;
}