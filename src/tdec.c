#include "inc/tdec.h"

#include <stdio.h>
#include <stdlib.h>

int tailleTdec = 0;

/* Affiche la nature d'une entree de la table des declarations */
static void affiche_nature_declaration(int nature) {
    switch (nature) {
        case NATURE_STRUCTURE:
            printf("structure");
            break;
        case NATURE_TABLEAU:
            printf("tableau");
            break;
        case NATURE_VARIABLE:
            printf("variable");
            break;
        case NATURE_PARAMETRE:
            printf("parametre");
            break;
        case NATURE_PROCEDURE:
            printf("procedure");
            break;
        case NATURE_FONCTION:
            printf("fonction");
            break;
        default:
            printf("inconnu");
            break;
    }
}

/* Ecrit les informations dans la table des declarations */
static void ecrit(int index, int nature, int region) {
    tdec[index].nature = nature;
    tdec[index].suivant = TDEC_VALEUR_NULL;
    tdec[index].region = region;
    tdec[index].index_trep = TDEC_VALEUR_NULL;
    tdec[index].exec = TDEC_VALEUR_NULL;

    tailleTdec++;
}

/* Affiche la table des declarations */
void tdec_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\tnature\t\t|\tsuivant\t|\tregion\t|\ttrep\t|\texec\n");
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
int tdec_insere(int index, int nature, int region) {
    if (index >= TDEC_TAILLE_MAX) {
        printf(
            "Erreur - La taille maximale de la table des declarations est "
            "atteinte.\n");
        return -1;
    }

    if (index < 0) {
        printf("Erreur - index de la table lexico < 0\n");
        return -1;
    }

    ecrit(index, nature, region);

    return index;
}