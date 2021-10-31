#include "inc/tlex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tailleTlex = 0;

/* Retourne la valeur de hash du lexème*/
static int hashLexeme(char *lexeme) {
    int i, sommeAscii = 0;
    for (i = 0; lexeme[i] != '\0'; i++) {
        sommeAscii += lexeme[i];
    }
    return (sommeAscii % THASH_MAX);
}

/* Ecrit dans la table de hashcode l'index du premier element de la table
 * lexicographique */
static void updateThash(int hashVal, int indexTlex) {
    if (thash[hashVal] == TLEX_VALEUR_NULL) {
        thash[hashVal] = indexTlex;
    }
}

/* Ecrit les informations du lexeme dans la table de hashcode et la table
 * lexicographique */
static void ecrit(int index, int hashVal, int taille, char *lexeme) {
    if (index >= TLEX_TAILLE_MAX) {
        printf(
            "Erreur - La taille maximale de la table lexicographique est "
            "atteinte.\n");
        return;
    }

    updateThash(hashVal, index);

    tlex[index].taille = taille;
    strcpy(tlex[index].lexeme, lexeme);
    tlex[index].suivant = TLEX_VALEUR_NULL;
}

/* initialise la table de hascode */
void initThash() {
    int i;
    for (i = 0; i < TLEX_TAILLE_MAX; i++) thash[i] = TLEX_VALEUR_NULL;
}

/* Retourne le lexème correspondant au numéro lexicographique */
char *lexeme(int numLexicographique) {
    if (numLexicographique < 0 || numLexicographique > TLEX_TAILLE_MAX)
        return NULL;
    if (tlex[numLexicographique].taille == 0) return NULL;

    return tlex[numLexicographique].lexeme;
}

/* Affiche la table lexicographique */
void tlex_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\ttaille\t|\tsuivant\t|\tlexeme\n");
    for (i = 0; i < tailleTlex; i++) {
        printf("%d\t|\t%d\t|\t%d\t|\t%s\n", i, tlex[i].taille, tlex[i].suivant,
               tlex[i].lexeme);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Affiche la table de hash-code */
void thash_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice/hash\t|\tindice tlex\n");
    for (i = 0; i < THASH_MAX; i++) {
        printf("%d\t\t|\t%d\n", i, thash[i]);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Insère le lexeme dans la table de hashcode et la table lexicographique */
int tlex_insere(char *lexeme) {
    int i;
    int returnVal = -1;
    int existeDeja = 0;
    int indexPrecedent = TLEX_VALEUR_NULL;
    int hashVal = hashLexeme(lexeme);
    int taille = strlen(lexeme);

    for (i = thash[hashVal]; i != TLEX_VALEUR_NULL; i = tlex[i].suivant) {
        if (!strcmp(lexeme, tlex[i].lexeme)) {
            existeDeja = 1;
            break;
        }
        indexPrecedent = i;
    }
    if (!existeDeja) {
        if (indexPrecedent != TLEX_VALEUR_NULL) {
            tlex[indexPrecedent].suivant = tailleTlex;
        }
        ecrit(tailleTlex, hashVal, taille, lexeme);
        returnVal = tailleTlex;
        tailleTlex++;
    }
    return returnVal;
}