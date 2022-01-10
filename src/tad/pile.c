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

/*
 * Detection de la presence ou non d'une valeur dans la pile
 */
int pile_est_dedans(pile p, int num) {
    int est_dedans = 0;
    int val;
    pile ptemp;
    pile_init(ptemp);

    while (!est_dedans && !pile_est_vide(p)) {
        val = pile_depile(p);
        pile_empile(ptemp, val);
        est_dedans = (val == num);
    }

    while (!pile_est_vide(ptemp)) {
        pile_empile(p, pile_depile(ptemp));
    }

    return est_dedans;
}

/*
 * Recuperation de la taille de la pile
 */
int pile_recupere_taille(pile p) { return p[0]; }