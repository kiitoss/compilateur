#include "inc/tdec.h"

#include <stdio.h>
#include <stdlib.h>

int tailleTdec = 0;

/* Ecrit les informations dans la table des declarations */
static void ecrit(int index, int nature) {
    if (index >= TDEC_TAILLE_MAX) {
        printf(
            "Erreur - La taille maximale de la table des declarations est "
            "atteinte.\n");
        return;
    }

    tdec[index].nature = nature;
    tdec[index].suivant = TDEC_VALEUR_NULL;
    tdec[index].region = TDEC_VALEUR_NULL;
    tdec[index].index_trep = TDEC_VALEUR_NULL;
    tdec[index].exec = TDEC_VALEUR_NULL;
}

/* Affiche la nature d'une entree de la table des declarations */
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
void tdec_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\tnature\t|\tsuivant\t|\tregion\t|\ttrep\t|\texec\n");
    for (i = 0; i < tailleTdec; i++) {
        printf("%d\t|\t", i);
        affiche_nature_declaration(tdec[i].nature);
        printf("\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", tdec[i].suivant,
               tdec[i].region, tdec[i].index_trep, tdec[i].exec);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Insère une nouvelle entree dans la table des declarations */
int tdec_insere() {
    return -1;
}