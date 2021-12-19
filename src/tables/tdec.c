#include "../../inc/tdec.h"

/* Taille de la table des declarations */
/* = index de la prochaine nouvelle entree */
int tdec_taille = TLEX_TMAX;

/* Table des declarations */
tdec_entree TDEC[TDEC_TMAX];

/*
 * Retourne sous forme de chaine de caractere la nature d'une declaration
 */
static char *tdec_recupere_nature_str(int nature) {
    switch (nature) {
        case -1:
            return "-1";
        case TYPE_B:
            return "TYPE_B";
        case TYPE_S:
            return "TYPE_S";
        case TYPE_T:
            return "TYPE_T";
        case VAR:
            return "VAR";
        case PROC:
            return "PROC";
        case FONC:
            return "FONC";
        default:
            return "inconnu";
    }
}

/*
 * Ecriture d'une nouvelle entree dans la table des declarations
 */
static void tdec_ecrit(int index, int nature, int region, int description, int execution) {
    /* cas d'erreur */
    if (index >= TDEC_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table des declarations est atteinte.\n");
        return;
    }

    TDEC[index].nature      = nature;
    TDEC[index].suivant     = VALEUR_NULL;
    TDEC[index].region      = region;
    TDEC[index].description = description;
    TDEC[index].execution   = execution;
}

/*
 * Initialisation de la table des declarations avec des valeurs nulles
 */
void tdec_init() {
    for (int i = 0; i < TDEC_TMAX; i++) {
        tdec_ecrit(i, VALEUR_NULL, VALEUR_NULL, VALEUR_NULL, VALEUR_NULL);
    }
}

/*
 * Insertion d'une nouvelle entree dans la table des declarations a partir d'un index lexicographique
 */
int tdec_nouvelle_entree(int tlex_index, int nature, int region, int description, int execution) {
    int index = tdec_taille;
    /*
     * index du champ precedent :
     * utile si l'insertion se fait dans la zone de debordement
     */
    int precedent;

    /* si aucun champ n'existe a l'index tlex_index : insertion et quitte la fonction */
    if (TDEC[tlex_index].nature == VALEUR_NULL) {
        tdec_ecrit(tlex_index, nature, region, description, execution);
        return tlex_index;
    }

    /* ecriture de la nouvelle entree dans la zone de debordement */
    tdec_ecrit(tdec_taille, nature, region, description, execution);

    /* mise a jour du champ "suivant" de l'entree precedente de meme lexeme */
    precedent = tlex_index;
    while (TDEC[precedent].suivant != VALEUR_NULL) {
        precedent = TDEC[precedent].suivant;
    }
    TDEC[precedent].suivant = tdec_taille;

    /* maj de la taille de la table des declarations */
    tdec_taille++;

    return index;
}

/*
 * Mise a jour de la taille a l'execution d'une entree de la table des declarations
 */
void tdec_maj_taille_exec(int index, int taille) {
    /* cas d'erreur */
    if (index > TDEC_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table des declarations est atteinte.\n");
        return;
    }

    TDEC[index].execution = taille;
}

/*
 * Recuperation de la taille a l'execution d'une entree de la table des declarations
 */
int tdec_recupere_taille_exec(int index) {
    /* cas d'erreur */
    if (index > TDEC_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table des declarations est atteinte.\n");
        return VALEUR_NULL;
    }

    return TDEC[index].execution;
}

/*
 * Affichage de la table des declarations
 */
void tdec_affiche() {
    /* permet l'affichage d'une ligne vide en cas de saut dans le tableau */
    int affiche_ligne_vide = 1;

    printf("-------------------------------------------------------------------\n");
    printf("index\t|nature\t|suivant|region\t\t|description\t|execution\n");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < tdec_taille; i++) {
        /* ignore les valeurs nulles */
        if (TDEC[i].nature == VALEUR_NULL && !affiche_ligne_vide) continue;

        printf("%d\t|%s\t|%d\t|%d\t\t|%d\t\t|%d\n", i, tdec_recupere_nature_str(TDEC[i].nature), TDEC[i].suivant,
               TDEC[i].region, TDEC[i].description, TDEC[i].execution);

        affiche_ligne_vide = (TDEC[i].nature != VALEUR_NULL);
    }

    printf("-------------------------------------------------------------------\n");
}