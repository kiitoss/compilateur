#ifndef TLEX_H
#define TLEX_H

#define TLEX_TAILLE_MAX 500
#define VALEUR_NULL -1
#define THASH_MAX 32

/* la structure d'une entrée dans la table lexicographique */
struct entree
{
  int taille;
  char *lexeme;
  int suivant;
};

struct entree tlex[TLEX_TAILLE_MAX];

int thash[THASH_MAX];

/* initialise la table de hascode */
void initThash();

/* Retourne le lexème correspondant au numéro lexicographique */
char *lexeme(int numLexicographique);

/* Affiche la table lexicographique */
void afficheTableLexico();

/* Insère le lexeme dans la table de hashcode et la table lexicographique */
int inserer(char *lexeme);

#endif