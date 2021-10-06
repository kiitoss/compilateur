%{
    #include <stdio.h>
		#include <stdlib.h>

    int yylex(void);
    void yyerror(char *);

		int line;
%}

// Déclaration des tokens
%token PROG
%token DEBUT FIN
%token POINT_VIRGULE DEUX_POINTS
%token VARIABLE IDF
%token ENTIER

// Tokens provisoires
%token AFFICHE

// Déclaration des associativité
%left '+' '-' '*' '/'

%%


	/* Le programme doit commencer par le token PROG */
program:	PROG corps
					;


	/** A modifier */
	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps:	liste_declarations liste_instructions
				| liste_instructions
        ;


	/** A modifier */
liste_declarations:	declaration_variable POINT_VIRGULE
										| liste_declarations declaration_variable POINT_VIRGULE
										;


	/** A modifier */
type_simple:	ENTIER
							;


	/** A modifier */
declaration_variable:	VARIABLE IDF DEUX_POINTS type_simple
											;


	/* Une liste d'instructions commence par le token DEBUT et se termine par le token FIN */
liste_instructions:	DEBUT suite_liste_inst FIN
										;


	/* La liste d'instructions est composée d'une ou plusieurs instructions terminé par le token POINT_VIRGULE */
suite_liste_inst:	instruction POINT_VIRGULE
									| suite_liste_inst instruction POINT_VIRGULE
									;

	/** A modifier */
instruction:	AFFICHE expression				{ printf("%d\n", $2); }
							;

	/** A modifier */
expression:
        		ENTIER                   							{ $$ = $1; }
        		| expression '+' expression           { $$ = $1 + $3; }
        		| expression '-' expression           { $$ = $1 - $3; }
						| expression '*' expression           { $$ = $1 * $3; }
						| expression '/' expression           { $$ = $1 / $3; }
        		;


%%

void yyerror(char *s) {
    fprintf(stderr, "ligne %d: %s\n", line, s);
		exit(-1);
}

int main(void) {
    yyparse();
    return 0;
}