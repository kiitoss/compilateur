%{
    #include <stdio.h>
    #include <stdlib.h>
	#include "inc/interpreteur.h"

    #ifndef AFFICHER_TABLES
	#define AFFICHER_TABLES 1
    #endif

    extern int line;
	
	int yylex(void);
	void yyerror(char *);


    int tlex_index = 0;

%}

%union {
    int entier;
    char *string;
}

%token DEBUT DEBUT_TLEX DEBUT_TDEC DEBUT_TREP DEBUT_TREG
%token FIN FIN_TLEX FIN_TDEC FIN_TREP FIN_TREG
%token SEPARATEUR

%token <entier> ENTIER
%token <string> LEXEME

%%
programme:
    | DEBUT DEBUT_TLEX table_lexicographique FIN_TLEX DEBUT_TDEC FIN_TDEC DEBUT_TREP FIN_TREP DEBUT_TREG FIN_TREG FIN
;

table_lexicographique:
    | table_lexicographique entree_tlex
;

entree_tlex: ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER SEPARATEUR LEXEME {
    tlex_ecrit($1, $3, $5, $7);
        // printf("%d\t%d\t%d\t%s\n", $1, $3, $5, $7);
    }
;
%%

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(EXIT_FAILURE);
}

int main(void) {
    init_tables();

	yyparse();

    if (AFFICHER_TABLES) {
		printf("\n\nAffichage de la table lexicographique:\n");
		tlex_affiche();
		/* printf("\n\nAffichage de la table des declarations:\n");
		tdec_affiche();
        printf("\n\nAffichage de la table des representations:\n");
		trep_affiche();
        printf("\n\nAffichage de la table des regions:\n");
		treg_affiche(); */
		printf("\n");
	} else {
		printf("Affichage des tables désactivé.\n");
	}
	
    return EXIT_SUCCESS;
}
