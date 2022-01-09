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

static int recupere_int_cellule(cellule c, int type) { return (type == 0) ? c.entier : (int) c.reel; }

static double recupere_double_cellule(cellule c, int type) { return (type == 0) ? (double) c.entier : c.reel; }

static cellule resoud_variable(arbre a, int type_retour) {
    cellule c = cellule_null();

    int tdec_index_variable      = association_nom(a->valeur_1);
    int tdec_index_type_variable = tdec_type_variable(tdec_index_variable);

    if (type_retour == 0) {
        c.entier =
            recupere_int_cellule(PEXEC[BC + tdec_recupere_taille_exec(tdec_index_variable)], tdec_index_type_variable);
    } else if (type_retour == 1) {
        c.reel = recupere_double_cellule(PEXEC[BC + tdec_recupere_taille_exec(tdec_index_variable)],
                                         tdec_index_type_variable);
    }
    return c;
}

static cellule resoud_operation(arbre a, cellule gauche, cellule droite, int type_retour) {
    cellule c = cellule_null();

    int nature = arbre_recupere_nature(a);

    if (nature == A_DIV && ((type_retour == 0 && droite.entier == 0) || (type_retour == 1 && droite.reel == 0))) {
        fprintf(stderr, "Division par 0 interdite !\n");
        exit(EXIT_FAILURE);
    }

    if (type_retour == 0) {
        switch (nature) {
            case A_PLUS:
                c.entier = gauche.entier + droite.entier;
                return c;
            case A_MOINS:
                c.entier = gauche.entier - droite.entier;
                return c;
            case A_MULT:
                c.entier = gauche.entier * droite.entier;
                return c;
            case A_DIV:
                c.entier = gauche.entier / droite.entier;
                return c;
            default:
                return c;
        }
    }

    if (type_retour == 1) {
        switch (nature) {
            case A_PLUS:
                c.reel = gauche.reel + droite.reel;
                return c;
            case A_MOINS:
                c.reel = gauche.reel - droite.reel;
                return c;
            case A_MULT:
                c.reel = gauche.reel * droite.reel;
                return c;
            case A_DIV:
                c.reel = gauche.reel / droite.reel;
                return c;
            default:
                return c;
        }
    }

    return c;
}

static cellule resoud_expression(arbre a, int type_retour) {
    cellule c = cellule_null();
    cellule gauche, droite;
    if (arbre_est_vide(a)) {
        fprintf(stderr, "Erreur - Resolution d'expression arithmetique vide impossible\n");
        return c;
    }
    switch (arbre_recupere_nature(a)) {
        case A_VAR:
            return resoud_variable(a, type_retour);
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
            gauche = resoud_expression(a->fils_gauche, type_retour);
            droite = resoud_expression(a->fils_gauche->frere_droit, type_retour);
            return resoud_operation(a, gauche, droite, type_retour);
        default:
            fprintf(stderr, "Cas expression arithmetique non gere\n");
            return c;
    }
}

static void parcours_arbre(arbre a) {
    if (arbre_est_vide(a)) {
        return;
    }

    if (arbre_recupere_nature(a) == A_AFFECT) {
        int tdec_index = association_nom(a->fils_gauche->valeur_1);
        int type       = tdec_type_variable(tdec_index);

        PEXEC[taille_pexec++] = resoud_expression(a->fils_gauche->frere_droit, type);
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