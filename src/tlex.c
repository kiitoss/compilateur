#include "../inc/tlex.h"

int taille_tlex = 0;

/* Retourne la valeur de hash du lexème*/
static int hash_lexeme(char *lexeme) {
    int i, somme_ascii = 0;
    for (i = 0; lexeme[i] != '\0'; i++) {
        somme_ascii += lexeme[i];
    }
    return (somme_ascii % THASH_TMAX);
}

/* Ecrit dans la table de hashcode l'index du premier element de la table
 * lexicographique */
static void maj_thash(int hash_val, int index_tlex) {
    if (thash[hash_val] == VALEUR_NULL) {
        thash[hash_val] = index_tlex;
    }
}

/* Ecrit les informations du lexeme dans la table de hashcode et la table
 * lexicographique */
static void ecrit(int index, int hash_val, int taille, char *lexeme) {
    if (index >= TLEX_TMAX) {
        printf(
            "Erreur - La taille maximale de la table lexicographique est "
            "atteinte.\n");
        return;
    }

    maj_thash(hash_val, index);

    tlex[index].taille = taille;
    strcpy(tlex[index].lexeme, lexeme);
    tlex[index].suivant = VALEUR_NULL;
}

/* initialise la table de hascode */
void init_thash() {
    int i;
    for (i = 0; i < TLEX_TMAX; i++) thash[i] = VALEUR_NULL;
}

/* Retourne le lexème correspondant au numéro lexicographique */
char *lexeme(int num_lexicographique) {
    if (num_lexicographique < 0 || num_lexicographique > TLEX_TMAX) return NULL;
    if (tlex[num_lexicographique].taille == 0) return NULL;

    return tlex[num_lexicographique].lexeme;
}

/* Affiche la table lexicographique */
void tlex_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\ttaille\t|\tsuivant\t|\tlexeme\n");
    for (i = 0; i < taille_tlex; i++) {
        printf("%d\t|\t%d\t|\t%d\t|\t%s\n", i, tlex[i].taille, tlex[i].suivant, tlex[i].lexeme);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Affiche la table de hash-code */
void thash_affiche() {
    int i, affiche_marque = 1;
    printf(
        "---------------------------------------------------------------------"
        "\nindice/hash\t|\tindice tlex\n");
    for (i = 0; i < THASH_TMAX; i++) {
        if (thash[i] == VALEUR_NULL) {
            if (affiche_marque) {
                affiche_marque = 0;
                printf("............\n");
            }
            continue;
        }
        affiche_marque = 1;
        printf("%d\t\t|\t%d\n", i, thash[i]);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Insère le lexeme dans la table de hashcode et la table lexicographique */
int tlex_insere(char *lexeme) {
    int i;
    int return_val      = VALEUR_NULL;
    int existe_deja     = 0;
    int index_precedent = VALEUR_NULL;
    int hash_val        = hash_lexeme(lexeme);
    int taille          = strlen(lexeme);

    for (i = thash[hash_val]; i != VALEUR_NULL; i = tlex[i].suivant) {
        if (!strcmp(lexeme, tlex[i].lexeme)) {
            existe_deja = 1;
            return_val  = i;
            break;
        }
        index_precedent = i;
    }
    if (!existe_deja) {
        if (index_precedent != VALEUR_NULL) {
            tlex[index_precedent].suivant = taille_tlex;
        }
        ecrit(taille_tlex, hash_val, taille, lexeme);
        return_val = taille_tlex;
        taille_tlex++;
    }
    return return_val;
}