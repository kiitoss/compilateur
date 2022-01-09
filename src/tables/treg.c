#include "../../inc/treg.h"

/* Taille de la table des regions */
/* = index de la prochaine nouvelle entree */
int treg_taille = 0;

/* Table des regions */
treg_entree TREG[TREG_TMAX];

/*
 * Ecriture d'une nouvelle entree dans la table des regions
 */
void treg_ecrit(int index, int taille, int nis) {
    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table des regions est atteinte.\n");
        return;
    }

    TREG[index].taille = taille;
    TREG[index].nis    = nis;

    /* maj de la taille de la table des regions */
    treg_taille++;
}

/*
 * Insertion d'une nouvelle entree dans la table des regions
 */
int treg_nouvelle_entree(int nis) {
    int taille;
    int index = treg_taille;

    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table des regions est atteinte.\n");
        return VALEUR_NULL;
    }

    if (index == 0) {
        taille = 0;
    } else {
        taille = 1 + nis;
    }

    treg_ecrit(index, taille, nis);
    treg_maj_arbre(index, arbre_creer_noeud_vide(A_NONE));

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
 * Mise à jour de l'abre d'une region
 */
void treg_maj_arbre(int index, arbre a) {
    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - MAJ de l'arbre de la region %d impossible.\n", index);
        return;
    }
    TREG[index].arbre = a;
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
    for (int i = 0; i < treg_taille; i++) {
        printf("---------------------\n");
        printf("DEBUT_ARBRE_REGION %d\n", i);
        printf("---------------------\n");
        arbre_affiche(TREG[i].arbre);
        printf("---------------------\n");
        printf("FIN_ARBRE_REGION\n");
        printf("---------------------\n\n");
    }

    printf("\n");
}

/*
 * Sauvegarde de la table des regions
 */
void treg_sauvegarde(FILE *f) {
    for (int i = 0; i < treg_taille; i++) {
        fprintf(f, "\t%d|%d|%d\n", i, TREG[i].taille, TREG[i].nis);
    }
    for (int i = 0; i < treg_taille; i++) {
        fprintf(f, "\n\t#DEBUT_ARBRE_REGION %d\n", i);
        arbre_sauvegarde(f, TREG[i].arbre, 0, 2);
        fprintf(f, "\t#FIN_ARBRE_REGION\n");
    }
}

/*
 * Recupere l'arbre d'une region
 */
arbre treg_recupere_arbre_region(int index) {
    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - Recuperation de l'arbre de la region %d impossible.\n", index);
        return NULL;
    }

    return TREG[index].arbre;
}

/*
 * Recupere le NIS d'une region
 */
int treg_recupere_nis_region(int index) {
    /* cas d'erreur */
    if (index >= TREG_TMAX) {
        fprintf(stderr, "Erreur - Recuperation de l'arbre de la region %d impossible.\n", index);
        return 0;
    }

    return TREG[index].nis;
}