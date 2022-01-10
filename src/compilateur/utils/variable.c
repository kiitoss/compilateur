#include "../../../inc/compilateur.h"

/*
 * Insertion d'une nouvelle variable dans TDEC
 */
void nouvelle_variable(int tlex_index, int tlex_index_type) {
    /* recuperation du numero de la region dans la pile de region */
    int region = pile_tete_de_pile(PREG);

    /* recuperation de la taille de la region courante */
    int taille_region = treg_recupere_taille(region);

    /* recuperation de l'index du type dans TDEC grace a l'index dans TLEX */
    int tdec_index_type = tdec_trouve_index(tlex_index_type, PREG);

    /* recuperation de la taille a l'execution du type de la variable dans TDEC*/
    int taille_exec = tdec_recupere_taille_exec(tdec_index_type);

    if (taille_exec == VALEUR_NULL) {
        fprintf(stderr, "(l:%d) Variable non initialisée ! (tlex: %d, tlex_type: %d)\n", line, tlex_index, taille_exec);
        exit(EXIT_FAILURE);
    }

    /* insertion de la variable dans TDEC */
    tdec_nouvelle_entree(tlex_index, VAR, region, tdec_index_type, taille_region);
    printf("TAILLE REG: %d\n", taille_region);
    treg_affiche();

    /* mise a jour de la taille de la region dans TREG */
    treg_maj_taille(region, taille_region + taille_exec);
}