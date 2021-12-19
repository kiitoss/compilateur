#include "../../inc/trep.h"

/* Taille de la table des representations */
/* = index de la prochaine nouvelle entree */
int trep_taille = TLEX_TMAX;

/* Table des representations */
int TREP[TREP_TMAX];

/*
 * Initialisation de la table des representations avec des valeurs nulles
 */
void trep_init() {
    for (int i = 0; i < TREP_TMAX; i++) {
        TREP[i] = VALEUR_NULL;
    }
}

/*
 * Insertion d'une nouvelle entree dans la table des representations
 */
int trep_nouvelle_entree(int valeur) {
    int index = trep_taille;

    /* cas d'erreur */
    if (trep_taille >= TREP_TMAX - 1) {
        fprintf(stderr, "Erreur - La taille maximale de la table des representations est atteinte.\n");
        return VALEUR_NULL;
    }

    TREP[index] = valeur;

    /* maj de la taille de la table des representations */
    trep_taille++;

    return index;
}

/*
 * Affichage de la table des representations
 */
void trep_affiche() {
    /* permet l'affichage d'une ligne vide en cas de saut dans le tableau */
    int affiche_ligne_vide = 1;

    printf("----------------\n");
    printf("index\t|valeur\n");
    printf("----------------\n");

    for (int i = 0; i < trep_taille; i++) {
        /* ignore les valeurs nulles */
        if (TREP[i] == VALEUR_NULL && !affiche_ligne_vide) continue;

        printf("%d\t|%d\n", i, TREP[i]);

        affiche_ligne_vide = (TREP[i] != VALEUR_NULL);
    }

    printf("----------------\n");
}