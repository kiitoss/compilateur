#include <stdio.h>
#include <stdlib.h>

#include "tlex.h"

/* Retourne la valeur de hash du lexème*/
static int hashLexeme(char *lexeme)
{
  int i, sommeAscii = 0;
  for (i = 0; lexeme[i] != '\0'; i++)
  {
    sommeAscii += lexeme[i];
  }
  return sommeAscii;
}

/* Retourne 1 les les deux lexèmes sont identiques, 0 sinon */
static int identiques(char *lexeme1, char *lexeme2)
{
  int i;
  int identiques = 1;

  for (i = 0; lexeme1[i] != '\0'; i++)
  {
    if (lexeme1[i] != lexeme2[i])
    {
      identiques = 0;
      break;
    }
  }

  return identiques;
}

/* Retourne la taille du lexème */
static int tailleLexeme(char *lexeme)
{
  int taille;
  for (taille = 0; lexeme[taille] != '\0'; taille++)
    ;
  return taille;
}

/* Ecrit les informations du lexeme dans la table de hashcode et la table lexicographique */
static void ecrit(int index, int hashVal, int taille, char *lexeme)
{
  thash[index] = hashVal;
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
  for (i = 0; i < TLEX_TAILLE_MAX && thash[i] != VALEUR_NULL; i++)
  {
    printf("%d\t|\t%d\t|\t%d\t|\t%s\n", i, tlex[i].taille, tlex[i].suivant, tlex[i].lexeme);
  }
  printf("---------------------------------------------------------------------\n");
}

/* Insère le lexeme dans la table de hashcode et la table lexicographique */
int inserer(char *lexeme)
{
  int i, hashVal, taille, returnVal = -1, ancienDernier = -1;
  hashVal = hashLexeme(lexeme);
  taille = tailleLexeme(lexeme);

  for (i = 0; i < TLEX_TAILLE_MAX; i++)
  {
    if (thash[i] == VALEUR_NULL)
    {
      ecrit(i, hashVal, taille, lexeme);
      if (ancienDernier != -1)
        tlex[ancienDernier].suivant = i;
      returnVal = i;
      break;
    }

    if (thash[i] == hashVal)
    {
      if (identiques(lexeme, tlex[i].lexeme))
        break;

      if (tlex[i].suivant != VALEUR_NULL)
      {
        i = tlex[i].suivant - 1;
      } else {
        ancienDernier = i;
      }
    }
  }
  return returnVal;
}