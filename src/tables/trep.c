#include "../../inc/trep.h"

/* Taille de la table des representations */
/* = index de la prochaine nouvelle entree */
int trep_taille = 0;

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
 * Ecriture d'une nouvelle entree dans la table des representations
 */
void trep_ecrit(int index, int valeur) {
    /* cas d'erreur */
    if (index >= TREP_TMAX) {
        fprintf(stderr, "Erreur TMAX - La taille maximale de la table des representations est atteinte.\n");
        return;
    }

    TREP[index] = valeur;

    /* maj de la taille de la table des representations */
    trep_taille++;
}

/*
 * Insertion d'une nouvelle entree dans la table des representations
 */
int trep_nouvelle_entree(int valeur) {
    int index = trep_taille;

    /* cas d'erreur */
    if (index >= TREP_TMAX) {
        fprintf(stderr, "Erreur TMAX - La taille maximale de la table des representations est atteinte.\n");
        return VALEUR_NULL;
    }

    trep_ecrit(index, valeur);

    return index;
}

/*
 * Mise a jour d'une valeur de la table des representations
 */
void trep_maj_valeur(int index, int valeur) {
    /* cas d'erreur */
    if (index > TREP_TMAX) {
        fprintf(stderr, "Erreur - MaJ a l'index %d de la table des representations impossible.\n", index);
        return;
    }

    TREP[index] = valeur;
}

/*
 * Recuperation d'une valeur de la table des representations
 */
int trep_recupere_valeur(int index) {
    /* cas d'erreur */
    if (index > TREP_TMAX) {
        fprintf(stderr, "Erreur - Recuperation a l'index %d de la table des representations impossible.\n", index);
        return VALEUR_NULL;
    }

    return TREP[index];
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

    for (int i = 0; i < TREP_TMAX; i++) {
        /* ignore les valeurs nulles */
        if (TREP[i] == VALEUR_NULL && !affiche_ligne_vide) continue;

        printf("%d\t|%d\n", i, TREP[i]);

        affiche_ligne_vide = (TREP[i] != VALEUR_NULL);
    }

    printf("----------------\n");
}