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
 * Resolution d'une affectation dans une structure
 */
static void resout_affectation_structure(arbre a) {
    int tlex_index       = a->valeur_1;
    int tlex_index_champ = a->valeur_2;

    int tdec_index                = tdec_trouve_index(tlex_index, PREG);
    int tdec_index_type_structure = tdec_recupere_description(tdec_index);
    int trep_index_type_structure = tdec_recupere_description(tdec_index_type_structure);

    int nb_champs_structure   = trep_recupere_valeur(trep_index_type_structure);
    int tdec_index_type_champ = -1;
    int index_champ           = 0;

    /* recuperation de l'index du type du champ dans la table des declarations */
    while (index_champ < nb_champs_structure) {
        if (trep_recupere_valeur(trep_index_type_structure + 1 + index_champ * 3) == tlex_index_champ) {
            tdec_index_type_champ = trep_recupere_valeur(trep_index_type_structure + 1 + index_champ * 3 + 1);
            break;
        }
        index_champ++;
    }

    /* si le type est null */
    if (tdec_index_type_champ == -1) {
        fprintf(stderr, "Erreur - Type du champ non declare\n");
        exit(EXIT_FAILURE);
    }

    /* calcul du resultat de l'expression */
    cellule resultat = resout_expression(a->frere_droit, tdec_index_type_champ);

    /* mise a jour dans la pile d'execution */
    PEXEC[pexec_index_structure(a)] = resultat;
}

/*
 * Resolution d'une affectation
 */
void resout_affectation(arbre a) {
    if (a->nature == A_VAR) {
        resout_affectation_variable(a);
    } else if (a->nature == A_LECTURE_TAB) {
        resout_affectation_tableau(a);
    } else if (a->nature == A_VAR_STRUCT) {
        resout_affectation_structure(a);
    }
}