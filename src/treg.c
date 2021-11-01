#include "inc/treg.h"

#include <stdio.h>
#include <stdlib.h>

int taille_treg = 0;

/* Initialise une nouvelle entree dans la table des regions */
static void init_treg_entree(int index, int nis) {
    treg[index].taille = 0;
    treg[index].nis = nis;
    treg[index].arbre = NULL;
}

/* Cree une nouvelle entree dans la table des regions */
int treg_nouvelle_entree(int nis) {
    int index = taille_treg;
    init_treg_entree(index, nis);
    taille_treg++;
    return index;
}

/* Affiche la table des regions */
void treg_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\ttaille\t|\tnis\t|\tarbre\n");
    for (i = 0; i < taille_treg; i++) {
        printf("%d\t|\t%d\t|\t%d\t|\t%p\n", i, treg[i].taille, treg[i].nis, treg[i].arbre);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}