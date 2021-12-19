#include "../../inc/pile.h"

/*
 * Initialisation de la pile
 */
void pile_init(pile p) { p[0] = 0; }

/*
 * Retourne 1 si la pile est vide, 0 sinon
 */
int pile_est_vide(pile p) { return p[0] == 0; }

/*
 * Empilage d'une nouvelle valeur dans la pile
 */
void pile_empile(pile p, int num) {
    /* cas d'erreur */
    if (p[0] >= PILE_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la pile est atteinte.\n");
        return;
    }

    p[0]++;
    p[p[0]] = num;
}

/*
 * Depilage d'une valeur dans la pile
 */
int pile_depile(pile p) {
    /* cas d'erreur */
    if (p[0] == 0) {
        fprintf(stderr, "Erreur - La pile est vide.\n");
        return VALEUR_NULL;
    }

    return p[p[0]--];
}

/*
 * Recuperation de la valeur en tete de pile
 */
int pile_tete_de_pile(pile p) {
    /* cas d'erreur */
    if (p[0] == 0) {
        fprintf(stderr, "Erreur - La pile est vide.\n");
        return VALEUR_NULL;
    }

    return p[p[0]];
}