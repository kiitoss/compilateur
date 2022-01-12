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
 * Recuperation de l'index de la cellule concerne par un appel a un tableau dans la pile d'execution
 */
int pexec_index_tableau(arbre a) {
    int tlex_index_idf = (int) a->fils_gauche->valeur_1;

    int tdec_index_idf                 = tdec_trouve_index(tlex_index_idf, PREG);
    int deplacement_exec               = tdec_recupere_taille_exec(tdec_index_idf);
    int tdec_index_type_tableau        = tdec_recupere_description(tdec_index_idf);
    int trep_index_tableau             = tdec_recupere_description(tdec_index_type_tableau);
    int tdec_index_type_entree_tableau = trep_recupere_valeur(trep_index_tableau);
    int taille_exec_entree_tableau     = tdec_recupere_taille_exec(tdec_index_type_entree_tableau);
    int trep_index_nb_dim              = trep_index_tableau + 1;
    int nb_dim                         = trep_recupere_valeur(trep_index_nb_dim);
    int index_dim                      = 0;

    int trep_index_dim, min_dim, max_dim, valeur_dim;
    pile pile_deplacements_exec;

    pile_init(pile_deplacements_exec);

    if (tdec_index_type_entree_tableau == -1) {
        fprintf(stderr, "Erreur - Type des entrees du tableau non declare\n");
        exit(EXIT_FAILURE);
    }

    for (arbre dim = a->fils_gauche->frere_droit; !arbre_est_vide(dim) && arbre_recupere_nature(dim) == A_DIM;
         dim       = dim->frere_droit) {
        trep_index_dim = trep_index_tableau + 2 + index_dim * 2;
        min_dim        = trep_recupere_valeur(trep_index_dim);
        max_dim        = trep_recupere_valeur(trep_index_dim + 1);

        valeur_dim = resout_expression(dim->fils_gauche, 0).entier;

        if (valeur_dim > max_dim) {
            fprintf(stderr, "Erreur - Indice superieur a la borne maximal de la dimension du tableau\n");
            exit(EXIT_FAILURE);
        }

        if (valeur_dim < min_dim) {
            fprintf(stderr, "Erreur - Indice inferieur a la borne minimal de la dimension du tableau\n");
            exit(EXIT_FAILURE);
        }

        valeur_dim -= min_dim;

        pile_empile(pile_deplacements_exec, max_dim - min_dim + 1);
        pile_empile(pile_deplacements_exec, valeur_dim);

        index_dim++;
    }

    if (index_dim != nb_dim) {
        fprintf(stderr, "Erreur - %d dimensions attendues, %d trouvees", nb_dim, index_dim);
        exit(EXIT_FAILURE);
    }

    int qte                      = pile_depile(pile_deplacements_exec);
    int taille                   = pile_depile(pile_deplacements_exec);
    int deplacement_exec_interne = qte;
    while (!pile_est_vide(pile_deplacements_exec)) {
        qte = pile_depile(pile_deplacements_exec);
        deplacement_exec_interne += (taille * qte);
        taille = pile_depile(pile_deplacements_exec);
    }

    deplacement_exec *= taille_exec_entree_tableau;
    return pexec_index_variable(tlex_index_idf, deplacement_exec) + deplacement_exec_interne;
}

/*
 * Recuperation de l'index de la cellule concerne par un appel a une structure dans la pile d'execution
 */
int pexec_index_structure(arbre a) {
    int tlex_index       = a->valeur_1;
    int tlex_index_champ = a->valeur_2;

    int tdec_index                = tdec_trouve_index(tlex_index, PREG);
    int tdec_index_type_structure = tdec_recupere_description(tdec_index);
    int trep_index_type_structure = tdec_recupere_description(tdec_index_type_structure);
    int deplacement_exec          = tdec_recupere_taille_exec(tdec_index);

    int nb_champs_structure   = trep_recupere_valeur(trep_index_type_structure);
    int tdec_index_type_champ = -1, deplacement_exec_interne = -1;
    int index_champ = 0;
    while (index_champ < nb_champs_structure) {
        if (trep_recupere_valeur(trep_index_type_structure + 1 + index_champ * 3) == tlex_index_champ) {
            tdec_index_type_champ    = trep_recupere_valeur(trep_index_type_structure + 1 + index_champ * 3 + 1);
            deplacement_exec_interne = trep_recupere_valeur(trep_index_type_structure + 1 + index_champ * 3 + 2);
        }
        index_champ++;
    }

    if (tdec_index_type_champ == -1 && deplacement_exec_interne == -1) {
        fprintf(stderr, "Erreur - Champ introuvable\n");
        exit(EXIT_FAILURE);
    } else if (tdec_index_type_champ == -1 || deplacement_exec_interne == -1) {
        fprintf(stderr, "Erreur - Lecture champ incomplete\n");
        exit(EXIT_FAILURE);
    }

    return pexec_index_variable(tlex_index, deplacement_exec) + deplacement_exec_interne;
}

static void controle_validite_params(arbre a) {
    pile params;
    pile_init(params);

    int tlex_index_param;

    if (arbre_est_vide(a) || a->nature != A_LISTE_PARAM) {
        fprintf(stderr, "Erreur - Liste de parametres attendus\n");
        exit(EXIT_FAILURE);
    }

    for (arbre param = a->fils_gauche; !arbre_est_vide(param) && param->nature == A_PARAM; param = param->frere_droit) {
        tlex_index_param = (int) param->valeur_1;
        if (pile_est_dedans(params, tlex_index_param)) {
            fprintf(stderr, "Erreur - Deux arguments possedent le meme identificateur\n");
            exit(EXIT_FAILURE);
        } else {
            pile_empile(params, tlex_index_param);
        }
    }
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
        } else {
            int tlex_index      = a->valeur_1;
            int tdec_index      = tdec_trouve_index(tlex_index, PREG);
            int tlex_index_type = tdec_recupere_description(tdec_index);
            int tdec_index_type = tdec_trouve_index(tlex_index_type, PREG);
            int taille_exec     = tdec_recupere_taille_exec(tdec_index_type);
            for (int i = 0; i < taille_exec; i++) {
                pexec_empile_entier(PEXEC, 0, &taille_pexec);
            }
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
        resout_affectation(a->fils_gauche);
    }
    /* si c'est un appel de procedure: execution de l'arbre de la procedure */
    else if (nature == A_APPEL) {
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
    /* si c'st une liste de parametres (declaration fonction/procedure) */
    else if (nature == A_LISTE_PARAM) {
        controle_validite_params(a);
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
    /* si c'est une condition*/
    else if (nature == A_CONDITION) {
        int expression_bool = resout_expression_booleenne(a->fils_gauche->fils_gauche);
        if (expression_bool) {
            parcours_arbre(a->fils_gauche->frere_droit);
        } else {
            parcours_arbre(a->fils_gauche->frere_droit->frere_droit);
        }
        parcours_arbre(a->frere_droit);
        return;
    }
    /* si c'est le resultat d'une condition */
    else if (nature == A_ALORS) {
        parcours_arbre(a->fils_gauche);
        return;
    }
    /* si c'est une ecriture sur la sortie standard */
    else if (nature == A_ECRIT) {
        resout_ecriture(a);
    }
    /* si c'est une lecture sur la sortie standard */
    else if (nature == A_LIRE) {
        resout_lecture(a);
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

    if (DEBOGAGE) {
        arbre_affiche(a);
    }

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
    if (DEBOGAGE) {
        fprintf(stdout, "\nAffichage de la pile d'execution:\n");
        pexec_affiche(PEXEC, taille_pexec);
        fprintf(stdout, "Fin de l'affichage de la pile d'execution\n\n");
    }
}