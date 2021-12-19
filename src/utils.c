#include "../inc/utils.h"

/* Variables globales */
int global_tlex_index = 0;  // index dans la table lexicographique
pile PREG;
global_tableau tableau;
global_structure structure;

/*
 * Insertion des types de base dans les differentes tables
 */
static void insere_types_base() {
    int index_tlex_entier    = tlex_nouvelle_entree("entier");
    int index_tlex_reel      = tlex_nouvelle_entree("reel");
    int index_tlex_booleen   = tlex_nouvelle_entree("booleen");
    int index_tlex_caractere = tlex_nouvelle_entree("caractere");
    tdec_nouvelle_entree(index_tlex_entier, TYPE_B, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_reel, TYPE_B, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_booleen, TYPE_B, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_caractere, TYPE_B, 0, VALEUR_NULL, 1);
}

static void nouvelle_region() {
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
 * Mise a jour de l'index global de la table lexicographique
 */
void maj_tlex_index(int tlex_index) { global_tlex_index = tlex_index; }

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
void nouveau_champ(int tlex_index) {
    /* recuperation de l'index du type dans TDEC grace a l'index dans TLEX */
    int tdec_index_type = tdec_trouve_index(tlex_index, PREG);

    /* ajout dans TREP de l'index du type de la structure dans TDEC */
    trep_nouvelle_entree(tdec_index_type);

    /* ajout dans TREP de l'index du type de la structure dans TLEX */
    trep_nouvelle_entree(tlex_index);

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