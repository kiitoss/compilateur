#include "../inc/utils.h"

/* Variables globales */
int global_tlex_index = 0;  // index dans la table lexicographique
int global_num_region = 0;  // numero de la region
global_tableau tableau;

/*
 * Insertion des types de base dans les differentes tables
 */
static void insere_types_base() {
    int index_tlex_entier    = tlex_nouvelle_entree("entier");
    int index_tlex_reel      = tlex_nouvelle_entree("reel");
    int index_tlex_booleen   = tlex_nouvelle_entree("booleen");
    int index_tlex_caractere = tlex_nouvelle_entree("caractere");
    tdec_nouvelle_entree(index_tlex_entier, 0, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_reel, 0, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_booleen, 0, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_caractere, 0, 0, VALEUR_NULL, 1);
}

/*
 * Inisitalisation des differentes tables
 */
void init_tables() {
    thash_init();
    tlex_init();
    tdec_init();
    trep_init();

    insere_types_base();
}

/*
 * Mise a jour de l'index global de la table lexicographique
 */
void maj_tlex_index(int tlex_index) { global_tlex_index = tlex_index; }

/*
 * Mise a jour du numero global de region
 */
void maj_num_region(int num_region) { global_num_region = num_region; }

/*
 * Insertion d'un nouveau tableau dans les differentes tables
 */
void debut_nouveau_tableau() {
    /* Reinitialisation de la variable globale du deplacement a l'execution */
    tableau.taille = 0;

    /*
     * Nouvelle entree TREP avec pour valeur 0: index TDEC du type du tableau
     * -> la modification se fera a la fin de la lecture du tableau
     */

    /*
     * Nouvelle entree TREP avec pour valeur 0: nombre de dimensions
     * -> la modification se fera au fur et a mesure de la lecture des parametres ou des dimensions
     */

    /*
     * Nouvelle entree TDEC avec pour champ exec 0: taille de la structure ou du tableau
     * -> la modification se fera au fur et a mesure de la lecture des parametres ou des dimensions
     */
}

/*
 * Insertion d'une nouvelle dimension d'un tableau dans les differentes tables
 * et mise a jour du tableau parent dans les differentes tables
 */
void nouvelle_dimension(int borne_min, int borne_max) {
    /* ajout dans la table des representations de la borne min de la dimension */
    trep_nouvelle_entree(borne_min);
    /* ajout dans la table des representations de la borne max de la dimension */
    trep_nouvelle_entree(borne_max);

    /* incrementation du nombre de dimensions du tableau parent dans la table des representations */

    /* mise a jour de la taille du tableau parent avec les nouvelles dimensions */
    if (tableau.taille == 0) {
        tableau.taille = (borne_max - borne_min);
    } else {
        tableau.taille *= (borne_max - borne_min);
    }
}

/*
 * Mise a jour du type et de la taille du tableau dans les differentes tables
 */
void fin_nouveau_tableau(int tdec_index_type) {
    /* mise a jour du type du tableau dans la table des representations */

    /* mise a jour de la taille du tableau dans la table des declarations */
}