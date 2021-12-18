#include "../../inc/thash.h"

/* Table de hash-code */
int THASH[THASH_TMAX];

/*
 * Generation d'un hash a partir d'un lexeme
 */
int genere_hash(char *lexeme) {
    int somme_ascii = 0;

    /* additionne la somme des caracteres du lexeme */
    for (int i = 0; lexeme[i] != '\0'; i++) {
        somme_ascii += (int) lexeme[i];
    }

    return (somme_ascii % THASH_TMAX);
}

/*
 * Initialisation de la table de hash-code avec des valeurs nulles
 */
void thash_init() {
    for (int i = 0; i < THASH_TMAX; i++) {
        THASH[i] = VALEUR_NULL;
    }
}

/*
 * Ecriture d'une nouvelle entree dans la table de hash-code
 */
void thash_ecrit(int index, int valeur) {
    /* cas d'erreur */
    if (index >= THASH_TMAX) {
        fprintf(stderr, "Erreur - L'index envoye est superieur a la taille de la table de hash-code.\n");
        return;
    }

    THASH[index] = valeur;
}

/*
 * Recuperation d'une entree de la table de hash-code
 */
int thash_recupere_valeur(int index) {
    /* cas d'erreur */
    if (index >= THASH_TMAX) {
        fprintf(stderr, "Erreur - L'index demande est superieur a la taille de la table de hash-code.\n");
        return VALEUR_NULL;
    }

    return THASH[index];
}

/*
 * Affichage de la table de hash-code
 */
void thash_affiche() {
    /* permet l'affichage d'une ligne vide en cas de saut dans le tableau */
    int affiche_ligne_vide = 1;

    printf("------------------------------------\n");
    printf("index/hash\t|index tlex\n");
    printf("------------------------------------\n");

    for (int i = 0; i < THASH_TMAX; i++) {
        if (THASH[i] == VALEUR_NULL && !affiche_ligne_vide) continue;

        printf("%d\t\t|%d\n", i, THASH[i]);

        affiche_ligne_vide = (THASH[i] != VALEUR_NULL);
    }

    printf("------------------------------------\n");
}