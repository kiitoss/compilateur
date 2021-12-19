#include "../../inc/treg.h"

/* Taille de la table des regions */
/* = index de la prochaine nouvelle entree */
int treg_taille = 0;

/* Table des regions */
treg_entree TREG[TREG_TMAX];

/*
 * Insertion d'une nouvelle entree dans la table des regions
 */
int treg_nouvelle_entree(int nis) {
    int index = treg_taille;

    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table des regions est atteinte.\n");
        return VALEUR_NULL;
    }

    TREG[index].taille = 0;
    TREG[index].nis    = nis;

    treg_taille++;
    return index;
}

/*
 * Recuperation de la taille d'une entree de la table des regions
 */
int treg_recupere_taille(int index) {
    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - Recuperation de l'index %d de la table des regions impossible.\n", index);
        return VALEUR_NULL;
    }

    return TREG[index].taille;
}

/*
 * Mise a jour de la taille d'une entree de la table des regions
 */
void treg_maj_taille(int index, int taille) {
    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - Modification de l'index %d de la table des regions impossible.\n", index);
        return;
    }

    TREG[index].taille = taille;
}

/*
 * Affichage de la table des regions
 */
void treg_affiche() {
    printf("---------------------\n");
    printf("index\t|taille\t|nis\n");
    printf("---------------------\n");

    for (int i = 0; i < treg_taille; i++) {
        printf("%d\t|%d\t|%d\n", i, TREG[i].taille, TREG[i].nis);
    }

    printf("---------------------\n");
}