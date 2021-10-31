%{
	#include "src/inc/tlex.h"
	#include "src/inc/tdec.h"

    #include <stdio.h>
	#include <stdlib.h>

    int yylex(void);
    void yyerror(char *);

	int line;
    int region = 0;
%}

// Déclaration des tokens
%token PROG
%token DEBUT FIN
%token POINT_VIRGULE DEUX_POINTS
%token VARIABLE IDF
%token FONCTION RETOURNE
%token PARENTHESE_OUVRANTE PARENTHESE_FERMANTE
%token ENTIER REEL BOOLEEN
%token PLUS MOINS MULT DIV
%token TRUE FALSE
%token ET OU NON
%token SI ALORS SINON

// Tokens provisoires
%token AFFICHE

%%


	/* Le programme doit commencer par le token PROG */
programme:	PROG corps
					;


	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps:	liste_declarations liste_instructions
				| liste_instructions
        ;


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
type_simple:    ENTIER
				| REEL
				| BOOLEEN
				;


	/* Grammaire de déclaration d'une variable */
declaration_variable:	VARIABLE IDF DEUX_POINTS nom_type   { tdec_insere($2, NATURE_VARIABLE, region); }
						;


	/* Grammaire de déclaration d'une fonction */
declaration_fonction:	FONCTION IDF liste_parametres RETOURNE type_simple corps    { region++; tdec_insere($2, NATURE_FONCTION, region);}
						;


	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres:
					| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
					;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param:	un_param
				| liste_param POINT_VIRGULE un_param
				;

	/* Grammaire d'un paramètre */
un_param:	IDF DEUX_POINTS type_simple
			;

	/** A modifier */
liste_variable:	expression									{ printf("%d\n", $1); }
              	| liste_variable POINT_VIRGULE expression	{ printf("%d\n", $3); }
              	;


	/** A modifier */
instruction:	AFFICHE expression	{ printf("%d\n", $2); }
				| condition
				;

	/** A modifier */
condition:	SI expression_booleenne
			ALORS liste_instructions
			SINON liste_instructions
			;


	/** A modifier */
expression:	ENTIER                   							        { $$ = $1; }
            | expression PLUS expression                                { $$ = $1 + $3; }
        	| expression MOINS expression                               { $$ = $1 - $3; }
			| expression MULT expression                                { $$ = $1 * $3; }
			| expression DIV expression                                 { $$ = $1 / $3; }
			| PARENTHESE_OUVRANTE liste_variable PARENTHESE_FERMANTE 
        	;


	/** A modifier */
expression_booleenne:	TRUE
						| FALSE
	                    | expression_booleenne ET expression_booleenne	{ $$ = $1 && $3; }
						| expression_booleenne OU expression_booleenne	{ $$ = $1 || $3; }
						| NON expression_booleenne						{ $$ = !$2; }
						;


%%

void yyerror(char *s) {
    fprintf(stderr, "ligne %d: %s\n", line, s);
		exit(-1);
}

int main(void) {
		initThash();
		initTdec();
    yyparse();
    printf("\n\nAffichage de la table de hash-code:\n");
    thash_affiche();
    printf("\n\nAffichage de la table lexicographique:\n");
    tlex_affiche();
    printf("\n\nAffichage de la table des déclarations:\n");
    tdec_affiche();
    return 0;
}