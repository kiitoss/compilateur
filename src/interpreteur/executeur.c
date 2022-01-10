#include "../../inc/interpreteur.h"

/* pile d'execution */
pexec PEXEC;
/* taille de la pile d'execution */
int taille_pexec = 0;

/* pile des regions */
pile PREG;

pile PFONC;

/* base courante dans la pile d'execution */
int BC = 0;

/*
 * bases courantes des differentes regions
 * permet de conserver les bases courantes des regions parentes
 */
int BC_regions[100];

/*
 * stocke le parent d'une region
 * ex: reg_parentes[1] = region parente de la region 1
 */
int reg_parentes[100];

/*
 * Execution d'une region = interpretation
 */
static void execute_region(int region);

static int resout_expression_booleenne(arbre a);

static void resout_appel(arbre a);

/*
 * Recuperation de la region de declaration d'une variable grace a son index lexicographique
 */
static int trouve_region_declare(int tlex_index) {
    int tdec_index = tdec_trouve_index(tlex_index, PREG);
    int region     = tdec_recupere_region(tdec_index);
    return region;
}

/*
 * Recuperation de l'index d'une variable dans la pile d'execution
 * a partir de son index lexicographique et de son deplacement a l'execution
 */
static int pexec_index_variable(int tlex_index, int deplacement_exec) {
    int region_declare = trouve_region_declare(tlex_index);
    int nis_declare    = treg_recupere_nis_region(region_declare);
    int nis_utilise    = treg_recupere_nis_region(pile_tete_de_pile(PREG));

    if (pile_tete_de_pile(PREG) == 0) {
        return deplacement_exec;
    } else if (nis_declare == nis_utilise) {
        printf("result: %d (%d)\n", BC + deplacement_exec, BC);
        return BC + deplacement_exec;
    }

    return PEXEC[BC + nis_utilise - nis_declare].entier + deplacement_exec;
}

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
static cellule resout_variable(arbre a, int type_retour) {
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

/*
 * Retourne une cellule integrant la valeur d'une operation
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
static cellule resout_operation(arbre a, cellule gauche, cellule droite, int type_retour) {
    /* initialisation de la cellule */
    cellule c = cellule_null();

    /* recuperation de la nature de l'operation */
    int nature = arbre_recupere_nature(a);

    /* verification de la validite de la division */
    if (nature == A_DIV && ((type_retour == 0 && droite.entier == 0) || (type_retour == 1 && droite.reel == 0))) {
        fprintf(stderr, "Division par 0 interdite !\n");
        exit(EXIT_FAILURE);
    }

    /* mise a jour de la cellule avec le resultat de l'operation */
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

/*
 * Retourne une cellule integrant la valeur d'une expression
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
static cellule resout_expression(arbre a, int type_retour) {
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
            printf("resout_expression\n");
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

/* */
static int resout_test_arithmetique(arbre a) {
    cellule gauche, droite;
    gauche = resout_expression(a->fils_gauche->fils_gauche, 1);
    droite = resout_expression(a->fils_gauche->fils_gauche->frere_droit, 1);
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

static int resout_expression_booleenne(arbre a) {
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

static void resout_appel(arbre a) {
    /* recuperation de la base courante de l'appelant */
    int BC_appelant = BC;

    /* mise a jour de la base courante */
    BC = taille_pexec;
    printf("decl BC: %d\n", BC);

    /* recuperation de l'index lexical de la procedure */
    int tlex_index = (int) a->fils_gauche->valeur_1;

    /* deduction de l'index de la procedure et de sa region dans la table des declarations */
    int tdec_index = tdec_trouve_index_fonction_procedure(tlex_index, PREG);

    if (tdec_recupere_nature(tdec_index) == FONC) {
        pile_empile(PFONC, tlex_index);
        /* empilage de la valeur de retour de la fonction */
        pexec_empile_entier(PEXEC, 0, &taille_pexec);
    }
    int region = tdec_recupere_taille_exec(tdec_index);

    /* empilage de la BC de l'appelant dans la pile d'execution (chainage) */
    pexec_empile_entier(PEXEC, BC_appelant, &taille_pexec);

    /* empilage la BC des parents dans la pile d'execution (chainage) */
    int reg_parent, BC_region_parent;
    reg_parent = reg_parentes[region];
    for (int i = 0; i < treg_recupere_nis_region(region); i++) {
        BC_region_parent = BC_regions[reg_parent];
        pexec_empile_entier(PEXEC, BC_region_parent, &taille_pexec);
        reg_parent = reg_parentes[reg_parent];
    }

    /* execution de la region */
    execute_region(region);

    /* mise a jour de la base courante pour revenir comme avant l'execution */
    BC = PEXEC[BC].entier;
}

/*
 * Parcours de l'arbre de la region avec execution
 */
static void parcours_arbre(arbre a) {
    int nature;

    if (arbre_est_vide(a)) return;

    /* recuperation de la nature du noeud */
    nature = arbre_recupere_nature(a);

    /* si c'est une declaration de variable: empile dans la pile d'execution */
    if (nature == A_DECL_VAR) {
        if (a->valeur_2 == 0) {
            pexec_empile_entier(PEXEC, 0, &taille_pexec);
        } else if (a->valeur_2 == 1) {
            pexec_empile_reel(PEXEC, 0, &taille_pexec);
        } else if (a->valeur_2 == 2) {
            pexec_empile_booleen(PEXEC, 0, &taille_pexec);
        } else if (a->valeur_2 == 3) {
            pexec_empile_caractere(PEXEC, 0, &taille_pexec);
        }
    }
    /* si c'est une declatation de procedure: met a jour la region parente */
    else if (nature == A_DECL_PROC || nature == A_DECL_FONC) {
        /* recuperation de l'index lexical de la procedure */
        int tlex_index = (int) a->valeur_1;

        /* deduction de l'index de la procedure et de sa region dans la table des declarations */
        int tdec_index = tdec_trouve_index(tlex_index, PREG);
        int region     = tdec_recupere_taille_exec(tdec_index);

        reg_parentes[region] = pile_tete_de_pile(PREG);
    }
    /* si c'est une affectation: maj dans la pile d'execution*/
    else if (nature == A_AFFECT) {
        /* recuperation de l'index lexical de l'idf */
        int tlex_index = (int) a->fils_gauche->valeur_1;

        /* deduction de l'index de la variable, du type de la variable et du deplacement a l'execution de la variable
         * dans la table des declarations */
        int tdec_index               = tdec_trouve_index(tlex_index, PREG);
        int tdec_index_type_variable = tdec_recupere_description(tdec_index);
        int deplacement_exec         = tdec_recupere_taille_exec(tdec_index);

        /* calcul du resultat de l'expression */
        cellule resultat = resout_expression(a->fils_gauche->frere_droit, tdec_index_type_variable);

        /* mise a jour dans la pile d'execution */
        PEXEC[pexec_index_variable(tlex_index, deplacement_exec)] = resultat;
    }
    /* si c'est un appel de procedure: execution de l'arbre de la procedure */
    else if (nature == A_APPEL) {
        printf("parcours_arbre\n");
        resout_appel(a);
        return;
    }
    /* si c'est un retour de fonction */
    else if (nature == A_RETOURNE) {
        int region        = pile_tete_de_pile(PREG);
        int base_courante = BC_regions[region];

        int tlex_index      = pile_depile(PFONC);
        int tdec_index      = tdec_trouve_index_fonction_procedure(tlex_index, PREG);
        int tdec_index_type = trep_recupere_valeur(tdec_recupere_description(tdec_index) + 0);

        /* calcul du resultat de retour */
        cellule resultat = resout_expression(a->fils_gauche, tdec_index_type);

        /* mise a jour dans la pile d'execution */
        PEXEC[base_courante + treg_recupere_nis_region(region) + 1] = resultat;
    }
    /* si c'est une boucle tant que */
    else if (nature == A_TANT_QUE) {
        int expression_bool = resout_expression_booleenne(a->fils_gauche);
        while (expression_bool == 1) {
            parcours_arbre(a->fils_gauche->frere_droit);
            expression_bool = resout_expression_booleenne(a->fils_gauche);
        }
        return;
    } else {
        /* execution recursive */
        parcours_arbre(a->fils_gauche);
        parcours_arbre(a->frere_droit);
        return;
    }

    /* execution recursive */
    parcours_arbre(a->frere_droit);
}

/*
 * Execution d'une region grace a son arbre
 */
static void execute_region(int region) {
    /* empilage de la nouvelle region dans la pile des regions */
    pile_empile(PREG, region);

    BC_regions[region] = BC;

    /* recuperation de l'arbre correspondant a la region */
    arbre a = treg_recupere_arbre_region(region);

    arbre_affiche(a);

    if (a->nature == A_DECL_PROC || a->nature == A_DECL_FONC) {
        a->nature = A_CORPS;
    }

    /* parcours de l'arbre avec interpretation */
    parcours_arbre(a);

    /* supprime la region de la pile des regions */
    pile_depile(PREG);
}

/*
 * Execution du programme apres recuperation des donnees
 */
void execution() {
    /* initialisation de la pile des regions */
    pile_init(PREG);

    /* initialisation de la pile des fonctions appelees */
    pile_init(PFONC);

    reg_parentes[0] = 0;

    /* execution de la region initiale (0) */
    execute_region(0);

    /* affichage de la pile d'execution */
    fprintf(stdout, "\nAffichage de la pile d'execution:\n");
    pexec_affiche(PEXEC, taille_pexec);
    fprintf(stdout, "Fin de l'affichage de la pile d'execution\n\n");
}