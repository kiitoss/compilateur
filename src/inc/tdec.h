#ifndef TDEC_H
#define TDEC_H

#define TDEC_TAILLE_MAX 500
#define TDEC_VALEUR_NULL -1

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
void afficheTableDeclarations();

#endif