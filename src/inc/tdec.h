#ifndef TDEC_H
#define TDEC_H

#define TDEC_TAILLE_MAX 500
#define TDEC_VALEUR_NULL -1

#define NATURE_STRUCTURE 0
#define NATURE_TABLEAU 1
#define NATURE_VARIABLE 2
#define NATURE_PARAMETRE 3
#define NATURE_PROCEDURE 4
#define NATURE_FONCTION 5

/* la structure d'une entrée dans la table des declarations */
struct tdec_entree {
    int nature;
    int suivant;
    int region;
    int index_trep;
    int exec;
};

struct tdec_entree tdec[TDEC_TAILLE_MAX];

/* Affiche la table des declarations */
void tdec_affiche();

/* Insère une nouvelle entree dans la table des declarations */
int tdec_insere(int index, int nature, int region);

#endif