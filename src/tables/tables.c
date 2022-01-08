#include "../../inc/tables.h"

/*
 * Inititalisation des differentes tables
 */
void init_tables() {
    thash_init();
    tlex_init();
    tdec_init();
    trep_init();
}

/*
 * Affichage des differentes tables
 */
void affiche_tables() {
    printf("\n\nAffichage de la table lexicographique:\n");
    tlex_affiche();
    printf("\n\nAffichage de la table des declarations:\n");
    tdec_affiche();
    printf("\n\nAffichage de la table des representations:\n");
    trep_affiche();
    printf("\n\nAffichage de la table des regions:\n");
    treg_affiche();
    printf("\n");
}

/*
 * Sauvegarde des tables
 */
void sauvegarde_tables(FILE *f) {
    fprintf(f, "#DEBUT\n\n");

    fprintf(f, "#DEBUT_TLEX\n");
    tlex_sauvegarde(f);
    fprintf(f, "#FIN_TLEX\n\n");

    fprintf(f, "#DEBUT_TDEC\n");
    tdec_sauvegarde(f);
    fprintf(f, "#FIN_TDEC\n\n");

    fprintf(f, "#DEBUT_TREP\n");
    trep_sauvegarde(f);
    fprintf(f, "#FIN_TREP\n\n");

    fprintf(f, "#DEBUT_TREG\n");
    treg_sauvegarde(f);
    fprintf(f, "#FIN_TREG\n\n");

    fprintf(f, "#FIN");
}