#include "../../../inc/interpreteur.h"

/*
 * Resolution d'une affectation dans une variable
 */
static void resout_affectation_variable(arbre a) {
    /* recuperation de l'index lexical de l'idf */
    int tlex_index = (int) a->valeur_1;

    /* deduction de l'index de la variable, du type de la variable et du deplacement a l'execution de la
     * variable dans la table des declarations */
    int tdec_index               = tdec_trouve_index(tlex_index, PREG);
    int tdec_index_type_variable = tdec_recupere_description(tdec_index);
    int deplacement_exec         = tdec_recupere_taille_exec(tdec_index);

    /* calcul du resultat de l'expression */
    cellule resultat = resout_expression(a->frere_droit, tdec_index_type_variable);

    /* mise a jour dans la pile d'execution */
    PEXEC[pexec_index_variable(tlex_index, deplacement_exec)] = resultat;
}

/*
 * Resolution d'une affectation dans un tableau
 */
static void resout_affectation_tableau(arbre a) {
    int tlex_index_idf = (int) a->fils_gauche->valeur_1;

    int tdec_index_idf                 = tdec_trouve_index(tlex_index_idf, PREG);
    int tdec_index_type_tableau        = tdec_recupere_description(tdec_index_idf);
    int trep_index_tableau             = tdec_recupere_description(tdec_index_type_tableau);
    int tdec_index_type_entree_tableau = trep_recupere_valeur(trep_index_tableau);

    /* calcul du resultat de l'expression */
    cellule resultat = resout_expression(a->frere_droit, tdec_index_type_entree_tableau);

    /* mise a jour dans la pile d'execution */
    PEXEC[pexec_index_tableau(a)] = resultat;
}

/*
 * Resolution d'une affectation
 */
void resout_affectation(arbre a) {
    resout_affectation_variable(a);
    if (a->nature == A_VAR) {
    } else if (a->nature == A_LECTURE_TAB) {
        resout_affectation_tableau(a);
    }
}