#include "../../../inc/interpreteur.h"

/*
 * Recuperation de la valeur d'une cellule sous forme d'un entier
 */
static int recupere_int_cellule(cellule c, int type) { return (type == 0) ? c.entier : (int) c.reel; }

/*
 * Recuperation de la valeur d'une cellule sous forme d'un reel
 */
static double recupere_double_cellule(cellule c, int type) { return (type == 0) ? (double) c.entier : c.reel; }

/*
 * Retourne une cellule integrant la valeur de la variable stockee dans l'arbre
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
cellule resout_variable(arbre a, int type_retour) {
    /* initialisation de la cellule */
    cellule c = cellule_null();

    /* recuperation de l'index lexicographique stocke dans l'arbre */
    int tlex_index = (int) a->valeur_1;

    /* deduction de l'index de la variable, du type de la variable et du deplacement a l'execution de la variable dans
     * la table des declarations */
    int tdec_index_variable      = tdec_trouve_index(tlex_index, PREG);
    int tdec_index_type_variable = tdec_recupere_description(tdec_index_variable);
    int deplacement_exec         = tdec_recupere_taille_exec(tdec_index_variable);

    /* recuperation de l'index de la variable dans la pile d'execution */
    int pexec_index = pexec_index_variable(tlex_index, deplacement_exec);

    /* mise a jour de la variable dans la pile d'execution avec un cast si necessaire */
    if (type_retour == 0) {
        c.entier = recupere_int_cellule(PEXEC[pexec_index], tdec_index_type_variable);
    } else if (type_retour == 1) {
        c.reel = recupere_double_cellule(PEXEC[pexec_index], tdec_index_type_variable);
    }

    return c;
}
