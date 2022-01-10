#include "../../../inc/interpreteur.h"

/*
 * Resolution d'un test arithmetique retournant 0 (faux) ou 1 (vrai)
 */
static int resout_test_arithmetique(arbre a) {
    /* resolution recursive */
    cellule gauche = resout_expression(a->fils_gauche->fils_gauche, 1);
    cellule droite = resout_expression(a->fils_gauche->fils_gauche->frere_droit, 1);

    switch (a->fils_gauche->nature) {
        case A_DOUBLE_EGAL:
            return gauche.reel == droite.reel;
        case A_SUP:
            return gauche.reel > droite.reel;
        case A_INF:
            return gauche.reel < droite.reel;
        case A_SUP_EGAL:
            return gauche.reel >= droite.reel;
        case A_INF_EGAL:
            return gauche.reel <= droite.reel;
        default:
            fprintf(stderr, "Cas test arithmetique non gere\n");
            return 0;
    }
}

/*
 * Calcul de la valeur d'une expression booleenne retournant 0 (faux) ou 1 (vrai)
 */
int resout_expression_booleenne(arbre a) {
    int gauche, droite;
    switch (a->fils_gauche->nature) {
        case A_TEST_ARITH:
            return resout_test_arithmetique(a->fils_gauche);
        case A_ET:
            gauche = resout_expression_booleenne(a->fils_gauche->fils_gauche);
            droite = resout_expression_booleenne(a->fils_gauche->fils_gauche->frere_droit);
            return (gauche && droite);
        case A_OU:
            gauche = resout_expression_booleenne(a->fils_gauche->fils_gauche);
            droite = resout_expression_booleenne(a->fils_gauche->fils_gauche->frere_droit);
            return (gauche || droite);
        case A_VRAI:
            return 1;
        case A_FAUX:
            return 0;
        default:
            fprintf(stderr, "Cas expression booleenne non gere\n");
            return 0;
    }

    return 0;
}
