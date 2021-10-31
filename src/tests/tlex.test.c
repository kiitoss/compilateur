#include "../inc/tlex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    initThash();
    printf("Affichage du tableau lexicograhique:\n");
    tlex_affiche();
    printf("\n");

    char lexeme1[] = "variable_1";
    char lexeme2[] = "variable_numero_1000";
    tlex_insere(lexeme1);
    tlex_insere(lexeme2);
    printf("Affichage du tableau lexicograhique:\n");
    tlex_affiche();
    printf("\n");

    tlex_insere(lexeme1);
    printf("Affichage du tableau lexicograhique:\n");
    tlex_affiche();
    printf("\n");

    char lexeme3[] = "arilabve_1";
    tlex_insere(lexeme3);
    printf("Affichage du tableau lexicograhique:\n");
    tlex_affiche();
    printf("\n");

    char lexeme4[] = "arilabev_1";
    tlex_insere(lexeme4);
    printf("Affichage du tableau lexicograhique:\n");
    tlex_affiche();
    printf("\n");

    return EXIT_SUCCESS;
}