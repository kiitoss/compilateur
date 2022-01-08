%{
    #include <stdio.h>
    #include <stdlib.h>
	#include "inc/interpreteur.h"

    #ifndef AFFICHER_TABLES
	#define AFFICHER_TABLES 1
    #endif

    extern int line;

    extern FILE *yyin;
	
	int yylex(void);
	void yyerror(char *);

    int nb_entrees_treg = 0;
    int nb_arbres_treg = 0;
%}

%union {
    int entier;
    char *string;
    arbre arbre;
}

%token DEBUT DEBUT_TLEX DEBUT_TDEC DEBUT_TREP DEBUT_TREG DEBUT_ARBRE_REGION
%token FIN FIN_TLEX FIN_TDEC FIN_TREP FIN_TREG FIN_ARBRE_REGION
%token SEPARATEUR CROCHET_OUVRANT CROCHET_FERMANT

%token <entier> ENTIER
%token <string> LEXEME

%type <arbre> treg_arbre treg_arbre_noeud
%%
programme:
    | DEBUT DEBUT_TLEX tlex FIN_TLEX DEBUT_TDEC tdec FIN_TDEC DEBUT_TREP trep FIN_TREP DEBUT_TREG treg_tableau treg_arbres FIN_TREG FIN {
        if (nb_entrees_treg != nb_arbres_treg) {
            fprintf(stderr, "Erreur - Nombre de regions != nombre d'arbres de regions\n");
            exit(EXIT_FAILURE);
        }
    }
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

treg_tableau:
    | treg_tableau treg_tableau_entree
;

treg_tableau_entree: ENTIER SEPARATEUR ENTIER SEPARATEUR ENTIER {
        nb_entrees_treg++;
        treg_ecrit($1, $3, $5);
    }
;

treg_arbres:
    | treg_arbres DEBUT_ARBRE_REGION ENTIER treg_arbre FIN_ARBRE_REGION {
        nb_arbres_treg++;
        treg_maj_arbre($3, $4);
    }
;

treg_arbre: {
        $$ = arbre_creer_noeud_vide(A_NONE);
    }
    | treg_arbre treg_arbre_noeud {
        $$ = arbre_concat_pere_fils($1, $2);
    }
;

treg_arbre_noeud: {
        $$ = arbre_creer_noeud_vide(A_NONE);
    }
    | treg_arbre_noeud ENTIER CROCHET_OUVRANT ENTIER CROCHET_FERMANT CROCHET_OUVRANT ENTIER CROCHET_FERMANT {
        $$ = arbre_creer_noeud_vide(A_NONE);
    }
;
%%

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(EXIT_FAILURE);
}

void usage(char *s) {
    fprintf(stderr, "Usage: %s input\n\tinput : fichier d'entree\n", s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s en lecture.\n", argv[1]);
        return EXIT_FAILURE;
    }

    init_tables();

	yyparse();

    if (AFFICHER_TABLES) {
		affiche_tables();
	} else {
		printf("Affichage des tables désactivé.\n");
	}
	
    fclose(yyin);

    return EXIT_SUCCESS;
}
