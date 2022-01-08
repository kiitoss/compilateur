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
    | DEBUT DEBUT_TLEX tlex FIN_TLEX DEBUT_TDEC tdec FIN_TDEC DEBUT_TREP trep FIN_TREP DEBUT_TREG treg FIN_TREG FIN
;

tlex:
    | tlex entree_tlex
;

entree_tlex: ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER SEPARATEUR LEXEME {
        tlex_ecrit($1, $3, $5, $7);
    }
;

tdec:
    | tdec entree_tdec
;

entree_tdec: ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER {
        tdec_ecrit($1, $3, $5, $7, $9, $11);
    }
;

trep:
    | trep entree_trep
;

entree_trep: ENTIER SEPARATEUR ENTIER {
        trep_ecrit($1, $3);
    }
;

treg:
    | treg entree_treg
;

entree_treg: ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER {
        treg_ecrit($1, $3, $5);
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
		affiche_tables();
	} else {
		printf("Affichage des tables désactivé.\n");
	}
	
    return EXIT_SUCCESS;
}
