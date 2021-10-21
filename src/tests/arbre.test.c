#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../arbre.h"

#define RANDOM_MAX 10

void usage(char *s)
{
  printf("Usage : %s prof\n", s);
  printf("\tprof : profondeur de l'arbre binaire à créer\n");
  exit(EXIT_FAILURE);
}

arbre genereArbreAleatoire(int profondeur) {
  int val;
  arbre a;

  if (profondeur == 0) return arbreVide();

  val = rand() % RANDOM_MAX + 1;
  a = creerNoeud(val);

  profondeur -= 1;
  a->gauche = genereArbreAleatoire(profondeur);
  a->droit = genereArbreAleatoire(profondeur);

  return a;
}

int main(int argc, char *argv[])
{
  int profondeur;
  arbre a;

  srand(time(NULL));

  if (argc < 2)
    usage(argv[0]);

  profondeur = atoi(argv[1]);

  a = genereArbreAleatoire(profondeur);

  afficheArbre(a);

  return EXIT_SUCCESS;
}