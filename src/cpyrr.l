%{
		#include "src/inc/tlex.h"
    #include "y.tab.h"
    #include <stdlib.h>

    void yyerror(char *);

		/* Définition de la constante permettant de pointer la ligne de l'erreur */
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
"DEBUT"						return DEBUT;
"FIN"							return FIN;
";"								return POINT_VIRGULE;
":"								return DEUX_POINTS;
"("								return PARENTHESE_OUVRANTE;
")"								return PARENTHESE_FERMANTE;
"affiche"					return AFFICHE;
"print"					return AFFICHE;
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
"int"             return ENTIER;
"float"           return REEL;
"bool"            return BOOLEEN;


	/* Déclaration des opérateurs */
\+                return PLUS;
\-                return MOINS;
\*                return MULT;
\/                return DIV; 


	/* Déclaration des nombres entiers */
[1-9]+{digit}*    {
										yylval = atoi(yytext);
										return ENTIER;
									}
  /* Déclaration des nombres réels */
[0-9]+,{digit}*		{
                    yylval = atof(yytext);
                  	return REEL;
									}

	/* A modifier */
{letter}+({letter}|{digit}|"_")*	{
																		yylval = tlex_insere(yytext);
																		return IDF;
																	}


	/* Cas d'erreur */
.           			yyerror("Caractère invalide !");


%%

int yywrap(void) {
    return 1;
}