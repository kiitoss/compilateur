#include <stdio.h>
#include <stdlib.h>

#include "arbre.h"

#define TAILLE_ESPACE 5

/* cree un noeud dont le champ noeud prend la valeur val
et le fils gauche et le frere droit la valeur NULL */
arbre creerNoeud(int val)
{
  arbre a = malloc(sizeof(arbre));
  a->noeud = val;
  return a;
}

/* retourne toujours la valeur NULL */
arbre arbreVide()
{
  return NULL;
}

/* Concatene a2 au frere droit de a1 */
arbre concatPereFrere(arbre a1, arbre a2)
{
  return arbreVide();
}

/* Concatene a2 au fils gauche de a1 */
arbre concatPereFils(arbre a1, arbre a2)
{
  return arbreVide();
}

/* Fonction auxiliaire d'affichage de l'arbre */
static void afficheArbreBis(arbre a, int espace)
{
  int i;
  if (a == arbreVide())
    return;

  espace += TAILLE_ESPACE;

  afficheArbreBis(a->droit, espace);

  printf("\n");
  for (i = TAILLE_ESPACE; i < espace; i++)
    printf(" ");
  printf("%d\n", a->noeud);

  afficheArbreBis(a->gauche, espace);
}

/* affiche l’arbre a */
void afficheArbre(arbre a)
{
  afficheArbreBis(a, 0);
}