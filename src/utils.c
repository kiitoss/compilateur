#include "../inc/utils.h"

/* Variables globales */
int global_tlex_index = 0;  // index dans la table lexicographique
pile PREG;

/*
 * Insertion des types de base dans les differentes tables
 */
static void insere_types_base() {
    int index_tlex_entier    = tlex_nouvelle_entree("int");
    int index_tlex_reel      = tlex_nouvelle_entree("float");
    int index_tlex_booleen   = tlex_nouvelle_entree("bool");
    int index_tlex_caractere = tlex_nouvelle_entree("char");
    tdec_nouvelle_entree(index_tlex_entier, TYPE_B, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_reel, TYPE_B, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_booleen, TYPE_B, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_caractere, TYPE_B, 0, VALEUR_NULL, 1);
}

/*
 * Insertion d'une nouvelle region dans PREG et dans TREG
 */
void nouvelle_region() {
    int region = treg_nouvelle_entree(pile_recupere_taille(PREG));
    pile_empile(PREG, region);
}

/*
 * Inisitalisation des differentes tables
 */
void init_tables() {
    thash_init();
    tlex_init();
    tdec_init();
    trep_init();

    /* insertion des types de base (entier/reel/booleen/char) */
    insere_types_base();

    /* initialisation de la pile et stockage de la premiere region */
    pile_init(PREG);
    nouvelle_region();
}

/*
 * Mise a jour de l'index global de TLEX
 */
void maj_tlex_index(int tlex_index) { global_tlex_index = tlex_index; }

/*
 * Recuperation l'index global de TLEX
 */
int recupere_tlex_index() { return global_tlex_index; }
