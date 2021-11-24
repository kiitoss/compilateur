#include "inc/tdec.h"

#include <stdio.h>
#include <stdlib.h>

int taille_tdec = TLEX_TAILLE_MAX;
int index_global_type_tdec = VALEUR_NULL;

/* Ecrit les informations dans la table des declarations */
static void ecrit(int index, int type_ou_index_trep, int nature, int region) {
    tdec[index].nature = nature;
    tdec[index].suivant = VALEUR_NULL;
    tdec[index].region = region;
    tdec[index].type_ou_index_trep = type_ou_index_trep;
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
        "\nindice\t|\tnature\t\t|\tsuivant\t|\tregion\t|\ttype|trep\t|\texec\n");
    for (i = 0; i < taille_tlex; i++) {
        printf("%d\t|\t", i);
        affiche_nature_declaration(tdec[i].nature);
        printf("\t|\t%d\t|\t%d\t|\t", tdec[i].suivant, tdec[i].region);
        if (tdec[i].nature != NATURE_VARIABLE) {
            printf("\t");
        }
        printf("%d", tdec[i].type_ou_index_trep);
        if (tdec[i].nature == NATURE_VARIABLE) {
            printf("\t");
        }
        printf("\t|\t%d\n", tdec[i].exec);
    }
    if (taille_tlex < TLEX_TAILLE_MAX) {
        printf("............\n");
    }
    for (i = TLEX_TAILLE_MAX; i < taille_tdec; i++) {
        printf("%d\t|\t", i);
        affiche_nature_declaration(tdec[i].nature);
        printf("\t|\t%d\t|\t%d\t|\t%d\t\t|\t%d\n", tdec[i].suivant,
               tdec[i].region, tdec[i].type_ou_index_trep, tdec[i].exec);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Insère une nouvelle entree dans la table des declarations */
int tdec_nouvelle_entree(int index_tlex, int type_ou_index_trep, int nature, int region) {
    int parent = -1;
    int existeDeja = 0;
    if (index_tlex == VALEUR_NULL) {
        printf("Erreur - index_tlex NULL \n");
        return -1;
    }

    if (index_tlex >= TDEC_TAILLE_MAX) {
        printf(
            "Erreur - La taille maximale de la table des declarations est "
            "atteinte.\n");
        return -1;
    }

    if (index_tlex < 0) {
        printf("Erreur - index de la table lexico < 0\n");
        return -1;
    }

    if (est_libre(index_tlex)) {
        ecrit(index_tlex, type_ou_index_trep, nature, region);
    } else {
        parent = index_tlex;
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
        ecrit(taille_tdec, type_ou_index_trep, nature, region);
        taille_tdec++;
    }

    return index_tlex;
}


/* Met à jour l'index global de la table des declarations du type */
void set_global_index_type_tdec(int index) {
    index_global_type_tdec = index;
}

/* Retourne l'index global du type dans la table des declarations */
int get_global_index_type_tdec() {
    return index_global_type_tdec;
}