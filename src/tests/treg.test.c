#include "../inc/treg.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Affichage de la table des régions:\n");
    treg_affiche();
    printf("\n");

    treg_nouvelle_entree(1);
    treg_nouvelle_entree(2);
    treg_nouvelle_entree(3);
    
    printf("Affichage de la table des régions:\n");
    treg_affiche();
    printf("\n");
    return EXIT_SUCCESS;
}