#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tlex.h"

/* Retourne la valeur de hash du lexème*/
static int hashLexeme(char *lexeme)
{
  int i, sommeAscii = 0;
  for (i = 0; lexeme[i] != '\0'; i++)
  {
    sommeAscii += lexeme[i];
  }
  return (sommeAscii % THASH_MAX);
}

int tailleTlex = 0;

/* Ecrit les informations du lexeme dans la table de hashcode et la table lexicographique */
static void ecrit(int index, int hashVal, int taille, char *lexeme)
{
  if (thash[hashVal] == VALEUR_NULL) thash[hashVal] = index;
  tlex[index].taille = taille;
  tlex[index].lexeme = lexeme;
  tlex[index].suivant = -1;
}

/* initialise la table de hascode */
void initThash()
{
  int i;
  for (i = 0; i < TLEX_TAILLE_MAX; i++)
    thash[i] = VALEUR_NULL;
}

/* Retourne le lexème correspondant au numéro lexicographique */
char *lexeme(int numLexicographique)
{
  if (numLexicographique < 0 || numLexicographique > TLEX_TAILLE_MAX)
    return NULL;
  if (tlex[numLexicographique].taille == 0)
    return NULL;

  return tlex[numLexicographique].lexeme;
}

/* Affiche la table lexicographique */
void afficheTableLexico()
{
  int i;
  printf("---------------------------------------------------------------------\n");
  for (i = 0; i < tailleTlex; i++)
  {
    printf("%d\t|\t%d\t|\t%d\t|\t%s\n", i, tlex[i].taille, tlex[i].suivant, tlex[i].lexeme);
  }
  printf("---------------------------------------------------------------------\n");
}

/* Insère le lexeme dans la table de hashcode et la table lexicographique */
int inserer(char *lexeme)
{
  int i, hashVal, taille, returnVal = -1;
  hashVal = hashLexeme(lexeme);
  taille = strlen(lexeme);

  if (thash[hashVal] == VALEUR_NULL)
  {
    if (tailleTlex < TLEX_TAILLE_MAX)
    {
      ecrit(tailleTlex, hashVal, taille, lexeme);
      returnVal = tailleTlex;
      tailleTlex++;
    }
  }
  else
  {
    int identique = 0;
    for (i = thash[hashVal]; tlex[i].suivant != VALEUR_NULL; i = tlex[i].suivant)
    {
      if (!strcmp(lexeme, tlex[i].lexeme))
      {
        identique = 1;
        break;
      }
    }
    if (!identique && strcmp(lexeme, tlex[i].lexeme) != 0)
    {
      tlex[i].suivant = tailleTlex;
      ecrit(tailleTlex, hashVal, taille, lexeme);
      returnVal = tailleTlex;
      tailleTlex++;
    }
  }

  return returnVal;
}