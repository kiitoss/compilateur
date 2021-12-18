%{
	#include "inc/tlex.h"
    #include "y.tab.h"

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
"DEBUT"							return DEBUT;
"FIN"							return FIN;
";"								return POINT_VIRGULE;
":"								return DEUX_POINTS;
","								return VIRGULE;
".."							return POINT_POINT;
"("								return PARENTHESE_OUVRANTE;
")"								return PARENTHESE_FERMANTE;
"["								return CROCHET_OUVRANT;
"]"								return CROCHET_FERMANT;
"affiche"						return AFFICHE;
"print"							return AFFICHE;
"PROG"							return PROG;
"var"							return VARIABLE;
"fonction"						return FONCTION;
"procedure"						return PROCEDURE;
"retourne"						return RETOURNE;
"et"							return ET;
"ou"							return OU;
"!"								return NON;
"si"							return SI;
"alors"							return ALORS;
"sinon"							return SINON;
"tantque"						return TANT_QUE;
"faire"							return FAIRE;
"type"							return TYPE;
"struct"						return STRUCT;
"fstruct"						return FSTRUCT;
"tableau"						return TABLEAU;
"de"							return DE;
"."								return POINT;
":="							return OPAFF;
"[^\"]+"						return CSTE_FORMAT;
"=="							return DOUBLE_EGAL;
"<"                             return INFERIEUR;
">"                             return SUPERIEUR;
">="                            return SUP_EGAL;
"<="                            return INF_EGAL;
"vrai"						{
										yylval.t_entier = 1;
										return TRUE;
									}
"faux"						{
										yylval.t_entier = 0;
										return FALSE;
									}
"int"             return ENTIER;
"float"           return REEL;
"bool"            return BOOLEEN;
"char"		   	  return CARACTERE;

	/* Déclaration des opérateurs */
\+                return PLUS;
\-                return MOINS;
\*                return MULT;
\/                return DIV; 


	/* Déclaration des nombres entiers */
[1-9]+{digit}*    {
										yylval.t_entier = atoi(yytext);
										return ENTIER;
									}
  /* Déclaration des nombres réels */
[0-9]+,{digit}+		{
                    yylval.t_entier = atof(yytext);
                  	return REEL;
									}

	/* A modifier */
{letter}+({letter}|{digit}|"_")*	{
																		yylval.t_entier = tlex_insere(yytext);
																		return IDF;
																	}


	/* Cas d'erreur */
.           			yyerror("Caractère invalide !");

%%

int yywrap(void) {
    return 1;
}