#ifndef PILE_H
#define PILE_H

#ifndef VALEUR_NULL
#define VALEUR_NULL -1
#endif

#include <stdio.h>

/* Taille maximale de la pile */
#define PILE_TMAX 50

typedef int pile[PILE_TMAX];

/* Initialisation de la pile des regions */
void pile_init(pile p);

/* Retourne 1 si la pile est vide, 0 sinon */
int pile_est_vide(pile p);

/* Empilage d'une nouvelle valeur dans la pile */
void pile_empile(pile p, int num);

/* Depilage d'une valeur dans la pile */
int pile_depile(pile p);

/* Recuperation de la valeur en tete de pile */
int pile_tete_de_pile(pile p);

/* Recuperation de la taille de la pile */
int pile_recupere_taille(pile p);

#endif