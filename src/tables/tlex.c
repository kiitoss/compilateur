#include "../../inc/tlex.h"

/* Taille de la table lexicographique */
/* = index de la prochaine nouvelle entree */
int tlex_taille = 0;

/* Table lexicographique  */
tlex_entree TLEX[TLEX_TMAX];

/*
 * Ecriture d'une nouvelle entree dans la table lexicographique
 */
void tlex_ecrit(int index, int taille, int suivant, char *lexeme) {
    /* cas d'erreur */
    if (index >= TLEX_TMAX) {
        fprintf(stderr, "Erreur - La taille maximale de la table lexicographique est atteinte.\n");
        return;
    }

    /* ajoute une nouvelle entree dans la table lexicographique */
    TLEX[index].taille  = taille;
    TLEX[index].suivant = suivant;
    strcpy(TLEX[index].lexeme, lexeme);

    /* maj de la taille de la table lexicographique */
    tlex_taille++;
}

/*
 * Initialisation de la table lexicographique avec des valeurs nulles
 */
void tlex_init() {
    for (int i = 0; i < TLEX_TMAX; i++) {
        TLEX[i].suivant = VALEUR_NULL;
    }
}

/*
 * Affichage de la table lexicographique
 */
void tlex_affiche() {
    /* permet l'affichage d'une ligne vide en cas de saut dans le tableau */
    int affiche_ligne_vide = 1;

    printf("---------------------------------------\n");
    printf("indice\t|taille\t|suiv\t|lexeme\n");
    printf("---------------------------------------\n");

    for (int i = 0; i < TLEX_TMAX; i++) {
        /* ignore les valeurs nulles */
        if (TLEX[i].taille == 0 && !affiche_ligne_vide) continue;

        printf("%d\t|%d\t|%d\t|%s\n", i, TLEX[i].taille, TLEX[i].suivant, TLEX[i].lexeme);

        affiche_ligne_vide = (TLEX[i].taille != 0);
    }

    printf("---------------------------------------\n");
}

/*
 * Insertion d'une nouvelle entree et recuperation de son index ou recuperation de l'index du lexeme dans la table
 * lexicographique
 */
int tlex_nouvelle_entree(char *lexeme) {
    /* index du lexeme precedent de meme hash-code */
    int precedent = VALEUR_NULL;

    /* stocke l'index de la premiere occurence dans la table lexicographique
     * du lexeme passe en parametre
     */
    int hash_code           = genere_hash(lexeme);
    int index_prem_occ_tlex = thash_recupere_valeur(hash_code);

    int index = index_prem_occ_tlex;

    while (index != VALEUR_NULL) {
        /* si le lexeme analyse est egal au lexeme passe en parametre, on quitte la boucle */
        if (!strcmp(lexeme, TLEX[index].lexeme)) break;

        precedent = index;
        index     = TLEX[index].suivant;
    }

    /*
     * si l'index n'est pas null, c'est qu'on est sorti de la boucle => le lexeme existe deja
     * s'il est nul, on stocke l'index de lexeme a inserer a la fin du tableau
     */
    if (index != VALEUR_NULL) {
        return index;
    } else {
        index = tlex_taille;
    }

    /* ecriture de la nouvelle entree */
    tlex_ecrit(index, strlen(lexeme), VALEUR_NULL, lexeme);

    /* lie ensemble un hash-code et un lexeme dans la table de hash-code */
    thash_ecrit(hash_code, index);

    /* mise a jour du champ "suivant" de l'entree precedente de meme lexeme */
    if (precedent != VALEUR_NULL) TLEX[precedent].suivant = index;

    return index;
}

/*
 * Sauvegarde de la table lexicographique
 */
void tlex_sauvegarde(FILE *f) {
    for (int i = 0; i < TLEX_TMAX; i++) {
        /* ignore les valeurs nulles */
        if (TLEX[i].taille == 0) continue;
        fprintf(f, "\t%d|%d|%d|%s\n", i, TLEX[i].taille, TLEX[i].suivant, TLEX[i].lexeme);
    }
}

/*
 * Recuperation d'un lexeme dans la table lexicographique
 */
char *tlex_recupere_lexeme(int index) {
    /* cas d'erreur */
    if (index >= TLEX_TMAX) {
        fprintf(stderr, "Erreur - Index %d de la table lexicographique inatteignable.\n", index);
        return "";
    }

    return TLEX[index].lexeme;
}