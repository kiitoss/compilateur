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

static void pexec_insere_args(arbre a) {
    if (arbre_est_vide(a)) {
        fprintf(stderr, "Erreur - Arbre vide, arguments attendus\n");
        exit(EXIT_FAILURE);
    }
    if (a->nature != A_LISTE_ARGS) {
        fprintf(stderr, "Erreur - Nature != liste arguments, arguments attendus\n");
        exit(EXIT_FAILURE);
    }

    /* WORK IN PROGRESS */
    // for (arbre param = a->fils_gauche; !arbre_est_vide(param) && param->nature == A_ARG; param = param->frere_droit)
    // {
    //     cellule resultat = resout_expression(param->fils_gauche, 1);
    //     pexec_empile(PEXEC, resultat, &taille_pexec);
    //     printf("EMPILE %f========\n", resultat.reel);
    // }
    /* WORK IN PROGRESS */
}

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
int pexec_index_variable(int tlex_index, int deplacement_exec) {
    int region_declare = trouve_region_declare(tlex_index);
    int nis_declare    = treg_recupere_nis_region(region_declare);
    int nis_utilise    = treg_recupere_nis_region(pile_tete_de_pile(PREG));

    if (pile_tete_de_pile(PREG) == 0) {
        return deplacement_exec;
    } else if (nis_declare == nis_utilise) {
        return BC + deplacement_exec;
    }

    return PEXEC[BC + nis_utilise - nis_declare].entier + deplacement_exec;
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
        pexec_insere_args(a->fils_gauche->frere_droit);
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
    }
    /* */
    else {
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
void execute_region(int region) {
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