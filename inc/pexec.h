#ifndef PILE_EXEC_H
#define PILE_EXEC_H

#include <stdio.h>
#include <stdlib.h>

/* Taille maximale de la pile d'execution */
#define PEXEC_TMAX 5000

/* Valeur NULL par defaut */
#ifndef VALEUR_NULL
#define VALEUR_NULL -1
#endif

union cellule {
    int entier;
    float reel;
    int booleen;
    char caractere;
};

typedef union cellule cellule;

typedef cellule pexec[PEXEC_TMAX];

/* Retourne une cellule vide */
cellule cellule_null();

// /* Retourne 1 si la pile est vide, 0 sinon */
// int pexec_est_vide(pexec p);

/* Empilage d'une nouvelle valeur dans la pile */
void pexec_empile(pexec p, cellule c, int *taille);

/* Empilage d'une nouvelle valeur entiere dans la pile */
void pexec_empile_entier(pexec p, int nb, int *taille);

/* Empilage d'une nouvelle valeur reel dans la pile */
void pexec_empile_reel(pexec p, double nb, int *taille);

/* Empilage d'une nouvelle valeur booleenne dans la pile */
void pexec_empile_booleen(pexec p, int bool, int *taille);

/* Empilage d'un nouveau caractere dans la pile */
void pexec_empile_caractere(pexec p, char carac, int *taille);

// /* Empilage d'un nouvel entier dans la pile */
// void pexec_empile_int(pexec p, int i);

/* Depilage d'une valeur dans la pile */
cellule pexec_depile(pexec p, int *taille);

// /* Recuperation de la valeur en tete de pile */
// cellule pexec_tete_de_pile(pexec p);

/* Affichage de la pile */
void pexec_affiche(pexec p, int max);

// /* Recuperation de la taille de la pile */
// int pexec_recupere_taille(pexec p);

#endif