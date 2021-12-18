#ifndef TLEX_H
#define TLEX_H

#include <string.h>

#include "global.h"

#define LEXEME_TAILLE_MAX 50

/* la structure d'une entrée dans la table lexicographique */
struct tlex_entree {
    int taille;
    char lexeme[LEXEME_TAILLE_MAX];
    int suivant;
};

struct tlex_entree tlex[TLEX_TMAX];

int thash[THASH_TMAX];

/* initialise la table de hascode */
void init_thash();

/* Retourne le lexème correspondant au numéro lexicographique */
char *lexeme(int numLexicographique);

/* Affiche la table lexicographique */
void tlex_affiche();

/* Affiche la table de hash-code */
void thash_affiche();

/* Insère le lexeme dans la table de hashcode et la table lexicographique */
int tlex_insere(char *lexeme);

#endif