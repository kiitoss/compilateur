#include "inc/tdec.h"

#include <stdio.h>
#include <stdlib.h>

int tailleTdec = TLEX_TAILLE_MAX;

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
            printf("null/inconnu");
            break;
    }
}

/* Ecrit les informations dans la table des declarations */
static void ecrit(int index, int nature, int region) {
    tdec[index].nature = nature;
    tdec[index].suivant = VALEUR_NULL;
    tdec[index].region = region;
    tdec[index].index_trep = VALEUR_NULL;
    tdec[index].exec = VALEUR_NULL;
}

/* Retourne 1 si l'index de la table des declarations est libre */
static int est_libre(int index) { return tdec[index].nature == VALEUR_NULL; }

/* initialise la table des declarations */
void initTdec() {
    int i;
    for (i = 0; i < TDEC_TAILLE_MAX; i++) {
        tdec[i].nature = VALEUR_NULL;
    }
}

/* Affiche la table des declarations */
void tdec_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\tnature\t\t|\tsuivant\t|\tregion\t|\ttrep\t|\texec\n");
    for (i = 0; i < tailleTlex; i++) {
        printf("%d\t|\t", i);
        affiche_nature_declaration(tdec[i].nature);
        printf("\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", tdec[i].suivant,
               tdec[i].region, tdec[i].index_trep, tdec[i].exec);
    }
    if (tailleTlex < TLEX_TAILLE_MAX) {
        printf("............\n");
    }
    for (i = TLEX_TAILLE_MAX; i < tailleTdec; i++) {
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
    int parent = -1;
    int existeDeja = 0;
    if (index == VALEUR_NULL) {
        printf("Erreur - index NULL \n");
        return -1;
    }

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

    if (est_libre(index)) {
        ecrit(index, nature, region);
    } else {
        parent = index;
        while (tdec[parent].suivant != VALEUR_NULL && !existeDeja) {
            if (tdec[parent].region == region) {
                existeDeja = 1;
                break;
            }
            parent = tdec[parent].suivant;
        }
        if (existeDeja || tdec[parent].region == region) {
            printf(
                "Erreur - une declaration de même type existe deja dans cette "
                "région.\n");
            return -1;
        }
        tdec[parent].suivant = tailleTdec;
        ecrit(tailleTdec, nature, region);
        tailleTdec++;
    }

    return index;
}