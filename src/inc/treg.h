#include "arbre.h"

#ifndef TREG_H
#define TREG_H

#define TREG_TAILLE_MAX 10

/* la structure d'une entrée dans la table des regions */
struct treg_entree {
    int taille;
    int nis;
    arbre arbre;
};

struct treg_entree treg[TREG_TAILLE_MAX];

/* Cree une nouvelle entree dans la table des regions */
int treg_nouvelle_entree(int nis);

/* Affiche la table des regions */
void treg_affiche();

#endif