#include "inc/tdec.h"

#include <stdio.h>
#include <stdlib.h>

int tailleTdec = 0;

static void affiche_nature_declaration(int nature) {
    switch (nature) {
        case 1:
            printf("structure");
            break;
        case 2:
            printf("tableau");
            break;
        case 3:
            printf("variable");
            break;
        case 4:
            printf("parametre");
            break;
        case 5:
            printf("procedure");
            break;
        case 6:
            printf("fonction");
            break;
        default:
            printf("inconnu");
            break;
    }
}

/* Affiche la table des declarations */
void afficheTableDeclarations() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nnature\t|\tsuivant\t|\tregion\t|\ttrep\t|\texec\n");
    for (i = 0; i < tailleTdec; i++) {
        affiche_nature_declaration(tdec[i].nature);
        printf("\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", tdec[i].suivant,
               tdec[i].num_region, tdec[i].index_trep, tdec[i].exec);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}