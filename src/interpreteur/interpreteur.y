%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
	#include "inc/interpreteur.h"

    /* Nombre maximal de noeuds */
    #define MAX_NOEUDS 100

    #define MAX_PROFONDEUR 10

    extern int line;

    extern FILE *yyin;
	
	int yylex(void);
	void yyerror(char *);

    int nb_entrees_treg = 0;
    int nb_arbres_treg = 0;

    arbre liste_noeuds[MAX_NOEUDS];
    int index_noeuds_parents[MAX_PROFONDEUR];
    int taille_liste_noeuds;
    int profondeur;

    void init_liste_noeuds();
    void reinitialise_noeuds_parents(int profondeur_min, int profondeur_max);
%}

%union {
    int entier;
    char *string;
    double reel;
    arbre arbre;
}

%token DEBUT DEBUT_TLEX DEBUT_TDEC DEBUT_TREP DEBUT_TREG DEBUT_ARBRE_REGION
%token FIN FIN_TLEX FIN_TDEC FIN_TREP FIN_TREG FIN_ARBRE_REGION
%token SEPARATEUR CROCHET_OUVRANT CROCHET_FERMANT

%token <entier> ENTIER
%token <reel> REEL
%token <string> LEXEME

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
    | treg_arbres DEBUT_ARBRE_REGION ENTIER { init_liste_noeuds(); } treg_arbre FIN_ARBRE_REGION {
        nb_arbres_treg++;
        treg_maj_arbre($3, liste_noeuds[index_noeuds_parents[0]]);
    }
;

treg_arbre:
    | treg_arbre treg_arbre_noeud
;

treg_arbre_noeud:
    | treg_arbre_noeud ENTIER SEPARATEUR ENTIER CROCHET_OUVRANT REEL CROCHET_FERMANT CROCHET_OUVRANT REEL CROCHET_FERMANT {
        int index = taille_liste_noeuds;
        liste_noeuds[index] = arbre_creer_noeud($4, $6, $9);
        taille_liste_noeuds++;

        reinitialise_noeuds_parents($2 + 1, profondeur);

        profondeur = $2;

        if (index_noeuds_parents[profondeur] == VALEUR_NULL) {
            index_noeuds_parents[profondeur] = index;

            if (profondeur > 0) {
                liste_noeuds[index_noeuds_parents[profondeur - 1]] = arbre_concat_pere_fils(
                    liste_noeuds[index_noeuds_parents[profondeur - 1]],
                    liste_noeuds[index]
                );
            }
        } else {
            liste_noeuds[index_noeuds_parents[profondeur]] = arbre_concat_pere_frere(
                liste_noeuds[index_noeuds_parents[profondeur]],
                liste_noeuds[index]
            );

            index_noeuds_parents[profondeur] = index;
        }
    }
;
%%

void reinitialise_noeuds_parents(int profondeur_min, int profondeur_max) {
    if (profondeur_max > MAX_PROFONDEUR) profondeur_max = MAX_PROFONDEUR;

    for (int i = profondeur_min; i <= profondeur_max; i++) {
        index_noeuds_parents[i] = VALEUR_NULL;
    }
}

void init_liste_noeuds() {
    taille_liste_noeuds = 0;
    profondeur = 0;
    reinitialise_noeuds_parents(0, MAX_PROFONDEUR);
}

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(EXIT_FAILURE);
}

void usage(char *s) {
    fprintf(stderr, "Usage: %s [options] input\n\tinput : fichier d'entree\n\n\tOPTIONS:\n\t\t-v : Affiche les tables et les arbres\n\t\t-h : Affiche l'aide\n", s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int opt;
    int verbose = 0;
    int nb_arguments;
    while((opt = getopt(argc, argv, "vh")) != -1) {
        switch(opt){
            case 'v':
                verbose = 1;
                break;
            case 'h':
                usage(argv[0]);
                break;
            case '?':
                usage(argv[0]);
                break;
        }
    }
    
    nb_arguments = argc - optind;
    
    if (nb_arguments < 1) {
        usage(argv[0]);
    }


    yyin = fopen(argv[optind], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s en lecture.\n", argv[optind]);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Debut de l'interpretation\n");

    init_tables();

	yyparse();

    if (verbose) {
		affiche_tables();
	}
	
    execution();

    fclose(yyin);

    fprintf(stdout, "Interpretation terminee\n");

    return EXIT_SUCCESS;
}
