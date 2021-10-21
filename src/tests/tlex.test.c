#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tlex.h"

int main(void) {
  initThash();
  printf("Affichage du tableau lexicograhique:\n");
  afficheTableLexico();
  printf("\n");

  char lexeme1[] = "variable_1";
  char lexeme2[] = "variable_numero_1000";
  inserer(lexeme1);
  inserer(lexeme2);
  printf("Affichage du tableau lexicograhique:\n");
  afficheTableLexico();
  printf("\n");

  inserer(lexeme1);
  printf("Affichage du tableau lexicograhique:\n");
  afficheTableLexico();
  printf("\n");

  char lexeme3[] = "arilabve_1";
  inserer(lexeme3);
  printf("Affichage du tableau lexicograhique:\n");
  afficheTableLexico();
  printf("\n");

  char lexeme4[] = "arilabev_1";
  inserer(lexeme4);
  printf("Affichage du tableau lexicograhique:\n");
  afficheTableLexico();
  printf("\n");

  return EXIT_SUCCESS;
}