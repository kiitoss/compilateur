#include "../../../inc/compilateur.h"

global_tableau tableau;

/*
 * Insertion d'un nouveau tableau dans les differentes tables
 */
void debut_nouveau_tableau() {
    /* Reinitialisation de la taille du tableau */
    tableau.taille = 0;

    /*
     * nouvelle entree TREP avec pour valeur 0: index TDEC du type du tableau
     * -> la modification se fera a la fin de la lecture du tableau
     */
    tableau.trep_index_type = trep_nouvelle_entree(0);

    /*
     * nouvelle entree TREP avec pour valeur 0: nombre de dimensions
     * -> la modification se fera au fur et a mesure de la lecture des dimensions
     */
    tableau.trep_index_nb_dimensions = trep_nouvelle_entree(0);

    /*
     * nouvelle entree TDEC avec pour champ exec 0: taille du tableau
     * -> la modification se fera au fur et a mesure de la lecture des dimensions
     */
    tableau.tdec_index =
        tdec_nouvelle_entree(global_tlex_index, TYPE_T, pile_tete_de_pile(PREG), tableau.trep_index_type, 0);
}

/*
 * Insertion d'une nouvelle dimension d'un tableau dans les differentes tables
 * et mise a jour du tableau parent dans les differentes tables
 */
void nouvelle_dimension(int borne_min, int borne_max) {
    /* ajout dans TREP de la borne min de la dimension */
    trep_nouvelle_entree(borne_min);
    /* ajout dans TREP de la borne max de la dimension */
    trep_nouvelle_entree(borne_max);

    /* incrementation du nombre de dimensions du tableau parent dans TREP */
    trep_maj_valeur(tableau.trep_index_nb_dimensions, trep_recupere_valeur(tableau.trep_index_nb_dimensions) + 1);

    /* mise a jour de la taille du tableau parent avec les nouvelles dimensions */
    if (tableau.taille == 0) {
        tableau.taille = (borne_max - borne_min + 1);
    } else {
        tableau.taille *= (borne_max - borne_min + 1);
    }
}

/*
 * Mise a jour du type et de la taille du tableau dans les differentes tables
 */
void fin_nouveau_tableau(int tlex_index_type) {
    /* recuperation de l'index du type dans TDEC grace a l'index dans TLEX */
    int tdec_index_type = tdec_trouve_index(tlex_index_type, PREG);

    int taille_type = tdec_recupere_taille_exec(tdec_index_type);

    /* mise a jour du type du tableau dans TREP */
    trep_maj_valeur(tableau.trep_index_type, tdec_index_type);

    /* mise a jour de la taille du tableau dans TDEC */
    tdec_maj_taille_exec(tableau.tdec_index, taille_type * tableau.taille);
}
