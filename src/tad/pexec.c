#include "../../inc/pexec.h"

// static cellule cellule_null() {
//     cellule c;
//     c.entier    = VALEUR_NULL;
//     c.reel      = VALEUR_NULL;
//     c.caractere = '0';
//     c.booleen   = '0';
//     return c;
// }

// /*
//  * Initialisation de la pile
//  */
// void pexec_init(pexec p) { p[0].entier = 0; }

// /*
//  * Retourne 1 si la pile est vide, 0 sinon
//  */
// int pexec_est_vide(pexec p) { return p[0].entier == 0; }

// /*
//  * Empilage d'une nouvelle valeur dans la pile
//  */
// void pexec_empile(pexec p, cellule c) {
//     /* cas d'erreur */
//     if (p[0].entier >= PEXEC_TMAX) {
//         fprintf(stderr, "Erreur - La taille maximale de la pile d'execution est atteinte.\n");
//         return;
//     }

//     p[0].entier++;
//     p[p[0].entier] = c;
// }

// /*
//  * Empilage d'un nouvel entier dans la pile
//  */
// void pexec_empile_int(pexec p, int i) {
//     cellule c = cellule_null();
//     c.entier  = i;
//     pexec_empile(p, c);
// }

// /*
//  * Depilage d'une valeur dans la pile
//  */
// cellule pexec_depile(pexec p) {
//     /* cas d'erreur */
//     if (p[0].entier == 0) {
//         fprintf(stderr, "Erreur - La pile d'execution est vide.\n");
//         return cellule_null();
//     }

//     return p[p[0].entier--];
// }

// /*
//  * Recuperation de la valeur en tete de pile
//  */
// cellule pexec_tete_de_pile(pexec p) {
//     /* cas d'erreur */
//     if (p[0].entier == 0) {
//         fprintf(stderr, "Erreur - La pile d'execution est vide.\n");
//         return cellule_null();
//     }

//     return p[p[0].entier];
// }

/*
 * Affichage de la pile
 */
void pexec_affiche(pexec p, int max) {
    for (int i = 0; i < max; i++) {
        printf("%d // %f // %c // %c\n", p[i].entier, p[i].reel, p[i].booleen, p[i].caractere);
    }
}

// /*
//  * Recuperation de la taille de la pile
//  */
// int pexec_recupere_taille(pexec p) { return p[0].entier; }