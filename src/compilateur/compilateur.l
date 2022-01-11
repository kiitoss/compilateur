%{
    #include "inc/compilateur.h"
	#include "inc/tlex.h"
    #include "inc/arbre.h"
    #include "y.tab.h"

    void yyerror(char *);

    /* Definition de la constante permettant de pointer la ligne de l'erreur */
    int line = 1;

    char *remplace_caractere(char* str, char ancien, char nouveau);
%}

digit   [0-9]
letter  [a-zA-Z]

%%


	/* Permet d'ignorer les commentaires */
"//".*  ;


	/* Permet d'ignorer les espaces */
[ \t]   ;


	/* Permet d'incrémenter la ligne de l'erreur */
\n { line++; }


	/* Déclaration des tokens */
";" { return POINT_VIRGULE; }
":" { return DEUX_POINTS; }
"," { return VIRGULE; }
"(" { return PARENTHESE_OUVRANTE; }
")" { return PARENTHESE_FERMANTE; }
"[" { return CROCHET_OUVRANT; }
"]" { return CROCHET_FERMANT; }
"." { return POINT; }
"<" { return INFERIEUR; }
">" { return SUPERIEUR; }
"!" { return NON; }

"DEBUT" { return DEBUT; }
"FIN"   { return FIN; }
"PROG"  { return PROG; }
"var"   { return VARIABLE; }
"et"    { return ET; }
"ou"    { return OU; }
"si"    { return SI; }
"alors" { return ALORS; }
"sinon" { return SINON; }
"faire" { return FAIRE; }
"type"  { return TYPE; }
"de"    { return DE; }
":="    { return OPAFF; }
"=="    { return DOUBLE_EGAL; }
">="    { return SUP_EGAL; }
"<="    { return INF_EGAL; }
"int"   { return TYPE_ENTIER; }
"float" { return TYPE_REEL; }
"bool"  { return TYPE_BOOLEEN; }
"char"  { return TYPE_CARACTERE; }
"ecrit" { return ECRIT; }
"\""    { return GUILLEMENT; }
".."    { return POINT_POINT; }
"ecrit" { return ECRIT; }
"lire"  { return LIRE; }

"fonction"  { return FONCTION; }
"procedure" { return PROCEDURE; }
"retourne"  { return RETOURNE; }
"tantque"   { return TANT_QUE; }
"struct"    { return STRUCT; }
"fstruct"   { return FSTRUCT; }
"tableau"   { return TABLEAU; }



    /* Déclaration d'une chaine de caractère */
\"([^\\\"]|\\.)*\"  { yylval.t_nombre = tlex_nouvelle_entree(yytext); return TEXTE; }

	/* Déclaration des opérateurs */
\+  { return PLUS; }
\-  { return MOINS; }
\*  { return MULT; }
\/  { return DIV; } 

"vrai"  { yylval.t_nombre = 1; return VRAI; }
"faux"  { yylval.t_nombre = 0; return FAUX; }

	/* Déclaration des nombres entiers */
{digit}+    { yylval.t_nombre = atof(yytext); return ENTIER; }

  /* Déclaration des nombres réels */
{digit}+,{digit}*		{ yylval.t_nombre = atof(remplace_caractere(yytext, ',', '.')); return REEL; }

	/* A modifier */
{letter}+({letter}|{digit}|"_")*	{ yylval.t_nombre = tlex_nouvelle_entree(yytext); return IDF; }

"'"{letter}"'" { yylval.t_nombre = (double) (yytext[1] - 'a'); return CARACTERE;}

{letter} { yylval.t_nombre = yytext[0] - 'a'; return CARACTERE; }
	/* Cas d'erreur */
.           			yyerror("Caractère invalide !");

%%

char *remplace_caractere(char* str, char ancien, char nouveau) {
    char *pos_actuelle = strchr(str, ancien);
    while (pos_actuelle) {
        *pos_actuelle = nouveau;
        pos_actuelle = strchr(pos_actuelle, ancien);
    }
    return str;
}

int yywrap(void) {
    return 1;
}