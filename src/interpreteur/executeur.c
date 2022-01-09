#include "../../inc/interpreteur.h"

void parcours_arbre(arbre a) {
    if (arbre_est_vide(a)) {
        return;
    }
}

/*
 * Execution du programme apres recuperation des donnees
 */
void execution() {
    printf("Execution\n");
    int region = 0;
    arbre a    = treg_recupere_arbre_region(region);
    arbre_affiche(a);
}