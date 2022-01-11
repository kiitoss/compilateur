#include "../../inc/pexec.h"

/*
 * Retourne une cellule vide
 */
cellule cellule_null() {
    cellule c;
    c.entier    = VALEUR_NULL;
    c.reel      = VALEUR_NULL;
    c.caractere = '0';
    c.booleen   = '0';
    return c;
}

/*
 * Empilage d'une nouvelle valeur dans la pile
 */
void pexec_empile(pexec p, cellule c, int *taille) {
    int index = *taille;
    /* cas d'erreur */
    if (index >= PEXEC_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la pile d'execution est atteinte.\n");
        return;
    }

    p[index].entier++;
    p[index] = c;

    (*taille)++;
}

/*
 * Empilage d'une nouvelle valeur entiere dans la pile
 */
void pexec_empile_entier(pexec p, int nb, int *taille) {
    cellule c = cellule_null();
    c.entier  = nb;
    pexec_empile(p, c, taille);
}

/*
 * Empilage d'une nouvelle valeur reel dans la pile
 */
void pexec_empile_reel(pexec p, double nb, int *taille) {
    cellule c = cellule_null();
    c.reel    = nb;
    pexec_empile(p, c, taille);
}

/*
 * Empilage d'une nouvelle valeur booleenne dans la pile
 */
void pexec_empile_booleen(pexec p, int bool, int *taille) {
    cellule c = cellule_null();
    c.booleen = bool;
    pexec_empile(p, c, taille);
}

/*
 * Empilage d'un nouveau caractere dans la pile
 */
void pexec_empile_caractere(pexec p, char carac, int *taille) {
    cellule c   = cellule_null();
    c.caractere = carac;
    pexec_empile(p, c, taille);
}

/*
 * Depilage d'une valeur dans la pile
 */
cellule pexec_depile(pexec p, int *taille) {
    int index = *taille;
    /* cas d'erreur */
    if (index == 0) {
        fprintf(stderr, "Erreur - La pile d'execution est vide.\n");
        return cellule_null();
    }

    (*taille)--;
    return p[(*taille)];
}

/*
 * Affichage de la pile
 */
void pexec_affiche(pexec p, int max) {
    for (int i = 0; i < max; i++) {
        printf("%d // %f // %c // %c\n", p[i].entier, p[i].reel, p[i].booleen, p[i].caractere);
    }
}