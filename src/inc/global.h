#ifndef GLOBAL_H
#define GLOBAL_H

#define THASH_TAILLE_MAX 32
/* TLEX_TAILLE_MAX doit être superieur ou egal a THASH_TAILLE_MAX */
#define TLEX_TAILLE_MAX 32
#define VALEUR_NULL -1

#define NATURE_TYPE_BASE 0
#define NATURE_STRUCTURE 1
#define NATURE_TABLEAU 2
#define NATURE_VARIABLE 3
#define NATURE_PARAMETRE 4
#define NATURE_PROCEDURE 5
#define NATURE_FONCTION 6

extern int taille_tlex;
extern int taille_trep;

/* Affiche la nature d'une entree de la table des declarations ou des representations */
void affiche_nature_declaration(int nature);

#endif