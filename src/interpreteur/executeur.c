#include "../../inc/interpreteur.h"

pexec PEXEC;
int taille_pexec = 0;

int BC = 0;

static int association_nom(int tlex_index) {
    int index = tlex_index;
    if (BC == 0) {
        return index;
    } else {
        return index;
    }
}

static int resoud_expression(arbre a) {
    if (arbre_est_vide(a)) {
        fprintf(stderr, "Erreur - Resolution d'expression arithmetique vide impossible\n");
        return 0;
    }
    switch (arbre_recupere_nature(a)) {
        int index;
        int gauche, droite, nature;
        case A_VAR:
            index = association_nom(a->valeur_1);
            return PEXEC[BC + tdec_recupere_taille_exec(index)].entier;
        case A_ENTIER:
            return a->valeur_1;
        case A_PLUS:
        case A_MOINS:
        case A_MULT:
        case A_DIV:
            gauche = resoud_expression(a->fils_gauche);
            droite = resoud_expression(a->fils_gauche->frere_droit);
            nature = arbre_recupere_nature(a);
            if (nature == A_PLUS) return gauche + droite;
            if (nature == A_MOINS) return gauche - droite;
            if (nature == A_MULT) return gauche * droite;
            if (nature == A_DIV) {
                if (droite == 0) {
                    fprintf(stderr, "Division par 0 interdite !\n");
                    exit(EXIT_FAILURE);
                }
                return gauche / droite;
            }
            return 0;
        default:
            fprintf(stderr, "Cas expression arithmetique non gere\n");
            return 0;
    }
}

static void parcours_arbre(arbre a) {
    if (arbre_est_vide(a)) {
        return;
    }

    if (arbre_recupere_nature(a) == A_AFFECT) {
        PEXEC[taille_pexec++].entier = resoud_expression(a->fils_gauche->frere_droit);
    }

    parcours_arbre(a->fils_gauche);
    parcours_arbre(a->frere_droit);
}

static void execute_region(int region) {
    arbre a = treg_recupere_arbre_region(region);
    arbre_affiche(a);
    parcours_arbre(a);
}

/*
 * Execution du programme apres recuperation des donnees
 */
void execution() {
    execute_region(0);
    fprintf(stdout, "\nAffichage de la pile d'execution:\n");
    pexec_affiche(PEXEC, taille_pexec);
    fprintf(stdout, "Fin de l'affichage de la pile d'execution\n\n");
}