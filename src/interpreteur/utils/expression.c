#include "../../../inc/interpreteur.h"

/*
 * Retourne une cellule integrant la valeur d'une expression
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
cellule resout_expression(arbre a, int type_retour) {
    /* initialisation de la cellule */
    cellule c = cellule_null();

    /* cellules representant les deux parties d'une operation */
    cellule gauche, droite;

    int tlex_index, tdec_index, base_courante, region;

    /* ca d'erreur (normalement inatteignable) */
    if (arbre_est_vide(a)) {
        fprintf(stderr, "Erreur - Resolution d'expression arithmetique vide impossible\n");
        return c;
    }

    /* resolution de l'expression */
    switch (arbre_recupere_nature(a)) {
        case A_VAR:
            return resout_variable(a, type_retour);
        case A_ENTIER:
        case A_REEL:
            if (type_retour == 0)
                c.entier = (int) a->valeur_1;
            else if (type_retour == 1)
                c.reel = a->valeur_1;
            return c;
        case A_PLUS:
        case A_MOINS:
        case A_MULT:
        case A_DIV:
            gauche = resout_expression(a->fils_gauche, type_retour);
            droite = resout_expression(a->fils_gauche->frere_droit, type_retour);
            return resout_operation(a, gauche, droite, type_retour);
        case A_EXPR_BOOL:
            c.booleen = resout_expression_booleenne(a);
            return c;
        case A_APPEL:
            resout_appel(a);
            tlex_index    = a->fils_gauche->valeur_1;
            tdec_index    = tdec_trouve_index(tlex_index, PREG);
            region        = tdec_recupere_taille_exec(tdec_index);
            base_courante = BC_regions[region];
            c             = PEXEC[base_courante + treg_recupere_nis_region(region) + 1];
            return c;
        default:
            fprintf(stderr, "Cas expression arithmetique non gere\n");
            return c;
    }
}
