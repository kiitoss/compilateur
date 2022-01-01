#ifndef TLEX_H
#define TLEX_H

#include <string.h>

#include "tables.h"
#include "thash.h"

#define LEXEME_TAILLE_MAX 50

/* Structure d'une ligne de la table lexicographique */
typedef struct {
    int taille;                      // taille du lexeme
    char lexeme[LEXEME_TAILLE_MAX];  // lexeme sous forme de string
    int suivant;                     // chainage sur le lexeme de meme hash-code
} tlex_entree;

/* Initialisation de la table lexicographique avec des valeurs nulles */
void tlex_init();

/* Affichage de la table lexicographique */
void tlex_affiche();

/* Ecriture d'une nouvelle entree dans la table lexicographique */
void tlex_ecrit(int index, int taille, int suivant, char *lexeme);

/*
 * Insertion d'une nouvelle entree et recuperation de son index ou recuperation de l'index du lexeme dans la table
 * lexicographique
 */
int tlex_nouvelle_entree(char *lexeme);

#endif