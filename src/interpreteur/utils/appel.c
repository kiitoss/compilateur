#include "../../../inc/interpreteur.h"

int taille_pexec_temp = 0;
pexec pexec_temp;

/*
 * Recuperation du nombre d'arguments lors d'un appel de fonction ou de procedure a partir du noeud a
 */
static int recupere_nb_arguments(arbre a) {
    /* stockage du nombre d'arguments */
    int nb_args = 0;

    /* si l'arbre est vide ou si ce n'est pas une liste d'arguments, c'est une erreur */
    if (arbre_est_vide(a) || arbre_recupere_nature(a) != A_LISTE_ARGS) {
        fprintf(stderr, "Erreur - Liste d'arguments attendue\n");
        exit(EXIT_FAILURE);
    }

    /* parcours des arguments */
    for (arbre arg = a->fils_gauche; !arbre_est_vide(arg) && arg->nature == A_ARG; arg = arg->frere_droit) {
        nb_args++;
    }

    return nb_args;
}

/*
 * Verification de la correspondance entre le nombre d'arguments passes en parametre
 * et le nombre d'arguments attendus
 */
static void controle_validite_nb_args(int tlex_index, int tdec_index, int nb_args) {
    /* recuperation du nombre d'arguments de l'appel de de la fonction/procedure*/
    int trep_deplacement = (tdec_recupere_nature(tdec_index) == FONC) ? 1 : 0;
    int trep_nb_params   = trep_recupere_valeur(tdec_recupere_description(tdec_index) + trep_deplacement);

    /* evaluation de la validite du nombre d'arguments passes en parametre */
    if (trep_nb_params != nb_args) {
        fprintf(stderr, "Erreur - Nombre de parametres invalides (%d au lieu de %d) (fonc_proc %d)\n", nb_args,
                trep_nb_params, tlex_index);
        exit(EXIT_FAILURE);
    }
}

/*
 * Depile les arguments de la pile d'execution vers une pile temporaire
 */
static void depile_args(int nb_args) {
    /* WORK IN PROGRESS */
    // for (int i = 0; i < nb_args; i++) {
    //     pexec_empile(pexec_temp, pexec_depile(PEXEC, &taille_pexec), &taille_pexec_temp);
    // }
    /* WORK IN PROGRESS */
}

/*
 * Empile les arguments de la pile temporaire vers la pile d'execution
 */
static void empile_args(int nb_args) {
    /* WORK IN PROGRESS */
    // for (int i = 0; i < nb_params; i++) {
    //     pexec_empile(PEXEC, pexec_depile(pexec_temp, &taille_pexec_temp), &taille_pexec);
    // }
    /* WORK IN PROGRESS */
}

/*
 * Empile le chainage dynamique et statique de la fonction/procedure
 */
static void empile_chainages(int BC_appelant, int region) {
    int reg_parent, BC_region_parent;

    /* empilage de la BC de l'appelant dans la pile d'execution (chainage) */
    pexec_empile_entier(PEXEC, BC_appelant, &taille_pexec);

    /* empilage la BC des parents dans la pile d'execution (chainage) */
    reg_parent = reg_parentes[region];
    for (int i = 0; i < treg_recupere_nis_region(region); i++) {
        BC_region_parent = BC_regions[reg_parent];
        pexec_empile_entier(PEXEC, BC_region_parent, &taille_pexec);
        reg_parent = reg_parentes[reg_parent];
    }
}

/*
 * Resolution d'un appel de fonction ou de procedure
 */
void resout_appel(arbre a) {
    /* recuperation de la base courante de l'appelant */
    int BC_appelant = BC;

    /* recuperation de l'index lexical de la procedure */
    int tlex_index = (int) a->fils_gauche->valeur_1;
    /* deduction de l'index de la procedure et de sa region dans la table des declarations */
    int tdec_index = tdec_trouve_index_fonction_procedure(tlex_index, PREG);
    /* recuperation du numero de region de la fonction/procedure*/
    int region = tdec_recupere_taille_exec(tdec_index);

    int nb_args = recupere_nb_arguments(a->fils_gauche->frere_droit);
    controle_validite_nb_args(tlex_index, tdec_index, nb_args);

    /* mise a jour de la base courante */
    BC = taille_pexec;

    depile_args(nb_args);

    empile_chainages(BC_appelant, region);

    if (tdec_recupere_nature(tdec_index) == FONC) {
        pile_empile(PFONC, tlex_index);
        /* empilage de la valeur de retour de la fonction */
        pexec_empile_entier(PEXEC, 0, &taille_pexec);
    }

    empile_args(nb_args);

    /* execution de la region */
    execute_region(region);

    /* mise a jour de la base courante pour revenir comme avant l'execution */
    BC = PEXEC[BC].entier;
}
