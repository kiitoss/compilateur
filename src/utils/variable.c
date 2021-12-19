#include "../../inc/utils.h"

/*
 * Insertion d'une nouvelle variable dans TDEC
 */
void nouvelle_variable(int tlex_index) {
    /* recuperation de l'index du type dans TDEC grace a l'index dans TLEX */
    int tdec_index_type = tdec_trouve_index(tlex_index, PREG);

    /* recuperation de la taille a l'execution du type de la variable dans TDEC*/
    int taille_exec = tdec_recupere_taille_exec(tdec_index_type);

    /* recuperation du numero de la region dans la pile de region */
    int region = pile_tete_de_pile(PREG);

    /* insertion de la variable dans TDEC */
    tdec_nouvelle_entree(tlex_index, VAR, region, taille_exec, treg_recupere_taille(region));

    /* mise a jour de la taille de la region dans TREG */
    treg_maj_taille(region, treg_recupere_taille(pile_tete_de_pile(PREG)) + taille_exec);
}