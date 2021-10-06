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
%token FONCTION RETOURNE
%token PARENTHESE_OUVRANTE PARENTHESE_FERMANTE
%token ENTIER

// Tokens provisoires
%token AFFICHE

// Déclaration des associativité
%left '+' '-' '*' '/'

%%


	/* Le programme doit commencer par le token PROG */
program:	PROG corps
					;


	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps:	liste_declarations liste_instructions
				| liste_instructions
        ;


	/** A modifier */
	/* Une liste de déclarations comprend une ou plusieurs déclarations séparées par le token POINT_VIRGULE */
liste_declarations:	declaration POINT_VIRGULE
										| liste_declarations declaration POINT_VIRGULE
										;


	/* Une liste d'instructions commence par le token DEBUT et se termine par le token FIN */
liste_instructions:	DEBUT suite_liste_inst FIN
										;


	/* La liste d'instructions est composée d'une ou plusieurs instructions terminé par le token POINT_VIRGULE */
suite_liste_inst:	instruction POINT_VIRGULE
									| suite_liste_inst instruction POINT_VIRGULE
									;


	/** A modifier */
/* Une déclaration peut être une déclaration de variable ou de fonction */
declaration:	declaration_variable
							| declaration_fonction
							;


	/* Le type peut être un type simple (entier, reel...) ou un identificateur (variable...) */
nom_type:	type_simple
					| IDF
					;

	/** A modifier */
type_simple:	ENTIER
							;


	/** A modifier */
declaration_variable:	VARIABLE IDF DEUX_POINTS nom_type
											;


	/** A modifier */
declaration_fonction:	FONCTION IDF liste_parametres RETOURNE type_simple corps
											;


	/** A modifier */
liste_parametres:
									| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
									;


	/** A modifier */
liste_param:	un_param
							| liste_param POINT_VIRGULE un_param
							;

	/** A modifier */
un_param:	IDF DEUX_POINTS type_simple
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