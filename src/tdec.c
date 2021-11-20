#include "inc/tdec.h"

#include <stdio.h>
#include <stdlib.h>

int taille_tdec = TLEX_TAILLE_MAX;

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
void init_tdec() {
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
    for (i = 0; i < taille_tlex; i++) {
        printf("%d\t|\t", i);
        affiche_nature_declaration(tdec[i].nature);
        printf("\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", tdec[i].suivant,
               tdec[i].region, tdec[i].index_trep, tdec[i].exec);
    }
    if (taille_tlex < TLEX_TAILLE_MAX) {
        printf("............\n");
    }
    for (i = TLEX_TAILLE_MAX; i < taille_tdec; i++) {
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
int tdec_nouvelle_entree(int index, int nature, int region) {
    int parent = -1;
    int existeDeja = 0;
    if (index == VALEUR_NULL) {
        printf("Erreur - index NULL \n");
        return -1;
    }

    if (index >= TDEC_TAILLE_MAX) {
        printf("tm: %d %d\n", index, TDEC_TAILLE_MAX);
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
        tdec[parent].suivant = taille_tdec;
        ecrit(taille_tdec, nature, region);
        taille_tdec++;
    }

    return index;
}