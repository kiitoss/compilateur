#include "../../../inc/compilateur.h"

global_structure structure;

/*
 * Insertion d'une nouvelle structure dans les differentes tables
 */
void debut_nouvelle_structure() {
    /* reinitialisation ddu deplacement a l'execution dans la structure */
    structure.deplacement_exec = 0;

    /*
     * nouvelle entree TREP avec pour valeur 0: nombre de champs
     * -> la modification se fera au fur et a mesure de la lecture des champs
     */
    structure.trep_index_nb_champs = trep_nouvelle_entree(0);

    /*
     * nouvelle entree TDEC avec pour champ exec 0: taille de la structure
     * -> la modification se fera au fur et a mesure de la lecture des champs
     */
    structure.tdec_index =
        tdec_nouvelle_entree(global_tlex_index, TYPE_S, pile_tete_de_pile(PREG), structure.trep_index_nb_champs, 0);
}

/*
 * Insertion d'un nouveau champ d'une structure dans les differentes tables
 * et mise a jour de la structure parente dans les differentes tables
 */
void nouveau_champ(int tlex_index, int tlex_index_type) {
    /* recuperation de l'index du type dans TDEC grace a l'index dans TLEX */
    int tdec_index_type = tdec_trouve_index(tlex_index_type, PREG);

    /* ajout dans TREP de l'index du champ dans TLEX */
    trep_nouvelle_entree(tlex_index);

    /* ajout dans TREP de l'index du type du champ dans TDEC */
    trep_nouvelle_entree(tdec_index_type);

    /* ajout dans TREP du deplacement a l'execution au sein de la structure */
    trep_nouvelle_entree(structure.deplacement_exec);

    /* incrementation du nombre de champs de la structure parente dans TREP */
    trep_maj_valeur(structure.trep_index_nb_champs, trep_recupere_valeur(structure.trep_index_nb_champs) + 1);

    /* incrementation du deplacement a l'execution */
    structure.deplacement_exec += tdec_recupere_taille_exec(tdec_index_type);
}

/*
 * Mise a jour de la taille de la structure dans TDEC
 */
void fin_nouvelle_structure() {
    /* mise a jour de la taille de la stucture dans TDEC */
    tdec_maj_taille_exec(structure.tdec_index, structure.deplacement_exec);
}