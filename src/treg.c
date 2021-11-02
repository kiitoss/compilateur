#include "inc/treg.h"

#include <stdio.h>
#include <stdlib.h>

int taille_treg = 0;
/* -1 car a l'initialisation, on fait id_region++ avant l'insertion*/
int id_region = -1;

int pile_regions[TREG_TAILLE_MAX];
int taille_pile_region = 0;

/* Retourne le nis */
static int get_nis() { return taille_pile_region; }

/* Augmente l'id de la region */
static void augmente_id_region() { id_region++; }

/* Initialise une nouvelle entree dans la table des regions */
static void init_treg_entree(int index) {
    treg[index].taille = 0;
    treg[index].nis = get_nis();
    treg[index].arbre = NULL;
}


/* Initialise la pile des regions */
void init_pile_regions() { treg_nouvelle_entree(); }

/* Retourne la region */
int get_region() { return pile_regions[taille_pile_region - 1]; }

/* Cree une nouvelle entree dans la table des regions */
void treg_nouvelle_entree() {
    int index = taille_treg;
    augmente_id_region();
    init_treg_entree(index);
    empile_region();
    taille_treg++;
}

/* Affiche la table des regions */
void treg_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\ttaille\t|\tnis\t|\tarbre\n");
    for (i = 0; i < taille_treg; i++) {
        printf("%d\t|\t%d\t|\t%d\t|\t%p\n", i, treg[i].taille, treg[i].nis,
               treg[i].arbre);
    }
    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Empile une nouvelle region dans la table des regions */
void empile_region() {
    if (taille_pile_region >= TREG_TAILLE_MAX) {
        printf("Erreur - Pile des régions a atteint son max d'elements.\n");
        return;
    }
    pile_regions[taille_pile_region++] = id_region;
}

/* Depile une region dans la table des regions */
int depile_region() {
    if (taille_pile_region == 0) {
        return -1;
    }
    return pile_regions[--taille_pile_region];
}