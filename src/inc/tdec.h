#include "global.h"

#ifndef TDEC_H
#define TDEC_H

#define TDEC_TAILLE_MAX TLEX_TAILLE_MAX*3

/* la structure d'une entrée dans la table des declarations */
struct tdec_entree {
    int nature;
    int suivant;
    int region;
    int index_trep;
    int exec;
};

struct tdec_entree tdec[TDEC_TAILLE_MAX];

/* initialise la table des declarations */
void init_tdec();

/* Affiche la table des declarations */
void tdec_affiche();

/* Insère une nouvelle entree dans la table des declarations */
int tdec_nouvelle_entree(int index, int nature, int region);

#endif