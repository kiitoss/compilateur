%{
    #include "inc/interpreteur.h"
    #include "y.tab.h"
    void yyerror(char *);

    /* Definition de la constante permettant de pointer la ligne de l'erreur */
    int line = 1;
%}

digit [0-9]
letter  [a-zA-Z]

%%
	/* Permet d'ignorer les espaces */
[ \t] ;

	/* Permet d'incrémenter la ligne de l'erreur */
\n { line++; }

"|" { return SEPARATEUR; }

"#DEBUT" { return DEBUT; }
"#FIN" { return FIN; }

"#DEBUT_TLEX" { return DEBUT_TLEX; }
"#FIN_TLEX" { return FIN_TLEX; }

"#DEBUT_TDEC" { return DEBUT_TDEC; }
"#FIN_TDEC" { return FIN_TDEC; }

"#DEBUT_TREP" { return DEBUT_TREP; }
"#FIN_TREP" { return FIN_TREP; }

"#DEBUT_TREG" { return DEBUT_TREG; }
"#FIN_TREG" { return FIN_TREG; }

"#DEBUT_ARBRE_REGION" { return DEBUT_ARBRE_REGION; }
"#FIN_ARBRE_REGION" { return FIN_ARBRE_REGION; }

"[" { return CROCHET_OUVRANT; }
"]" { return CROCHET_FERMANT; }

[+-]?{digit}+    { yylval.entier = atoi(yytext); return ENTIER; }
[+-]?{digit}+"."{digit}+    { yylval.reel = atof(yytext); return REEL; }
{letter}+({letter}|{digit}|"_")*	{ yylval.string = yytext; return LEXEME; }
\"([^\\\"]|\\.)*\"  { yylval.string = yytext; return LEXEME; }

	/* Cas d'erreur */
. yyerror("Caractère invalide !");

%%

int yywrap(void) {
    return 1;
}