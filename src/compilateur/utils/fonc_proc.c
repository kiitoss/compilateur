#include "../../../inc/compilateur.h"

global_fonc_proc fonc_proc;

/*
 * Insertion d'une nouvelle fonction ou procedure dans les differentes tables
 */
void debut_nouvelle_fonction_ou_procedure(int type, int tlex_index) {
    int trep_index_fonc_proc;

    /* Si le type est une fonction, insertion de son type */
    if (type == FONC) {
        fonc_proc.trep_index_type = trep_nouvelle_entree(0);
    }

    fonc_proc.trep_index_nb_parametres = trep_nouvelle_entree(0);

    /* Si le type est une fonction, son index dans TREP est l'index de son type
     * sinon son index dans TREP est l'index de son nb de params
     */
    if (type == FONC) {
        trep_index_fonc_proc = fonc_proc.trep_index_type;
    } else {
        trep_index_fonc_proc = fonc_proc.trep_index_nb_parametres;
    }

    /* nouvelle entree TDEC */
    fonc_proc.tdec_index = tdec_nouvelle_entree(tlex_index, type, pile_tete_de_pile(PREG), trep_index_fonc_proc, 0);

    /* changement de region */
    nouvelle_region();

    /*
     * liberation d'un espace pour le stockage de la variable de retour
     * note: on laisse ici un espace de 1 disponible car on impose que le retour d'une fonction soit un type simple
     */
    if (type == FONC) {
        treg_maj_taille(pile_tete_de_pile(PREG), treg_recupere_taille(pile_tete_de_pile(PREG)) + 1);
    }

    /* mise a jour du champ "execution" de la fonction ou de la procedure avec le numero de la region */
    tdec_maj_taille_exec(fonc_proc.tdec_index, pile_tete_de_pile(PREG));
}

/*
 * Insertion d'un nouveau parametre d'une fonction ou d'une procedure dans les differentes tables
 */
void nouveau_parametre(int tlex_index, int tlex_index_type) {
    /* le type d'un parametre ne peut etre qu'un type de base, c'est donc son numero lexical */
    int tdec_index_type = tlex_index_type;

    /* ajout dans TREP de l'index du parametre dans TLEX */
    trep_nouvelle_entree(tlex_index);

    /* ajout dans TREP de l'index du type du parametre dans TDEC */
    trep_nouvelle_entree(tdec_index_type);

    /* incrementation du nombre de parametres de la fonction ou procedure parente dans TREP */
    trep_maj_valeur(fonc_proc.trep_index_nb_parametres, trep_recupere_valeur(fonc_proc.trep_index_nb_parametres) + 1);

    nouvelle_variable(tlex_index, tdec_index_type);
}

/*
 * Mise a jour du type de retour d'une fonction et depile la region dans TDEC
 */
void fin_nouvelle_fonction_ou_procedure(int type, int tlex_index_type) {
    if (type == FONC) {
        /* recuperation de l'index du type dans TDEC grace a l'index dans TLEX */
        int tdec_index_type = tdec_trouve_index(tlex_index_type, PREG);

        /* mise a jour du type de retour de la fonction dans TREP */
        trep_maj_valeur(fonc_proc.trep_index_type, tdec_index_type);
    }
}

/*
 * Dépile la region en cours apres etre sortie de la fonction ou de la procedure
 */
void quitte_nouvelle_fonction_ou_procedure() {
    /* depile la region en cours */
    pile_depile(PREG);
}