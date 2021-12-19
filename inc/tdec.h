#ifndef TDEC_H
#define TDEC_H

#include "global.h"
#include "pile.h"

/* Taille maximale de la table des declarations */
/* = TLEX_TMAX + zone de debordement */
#define TDEC_TMAX 5000

/* Structure d'une ligne de la table des declarations */
typedef struct {
    int nature;       // structure, tableau, variable...
    int suivant;      // chainage sur la declaration de meme nom (zone de debordement)
    int region;       // numero de la region contenant la declaration
    int description;  // index dans TDEC ou TREP (selon la nature)
    int execution;    // taille ou deplacement a l'execution (selon la nature)
} tdec_entree;

/* Initialisation de la table des declarations avec des valeurs nulles */
void tdec_init();

/* Insertion d'une nouvelle entree dans la table des declarations a partir d'un index lexicographique */
int tdec_nouvelle_entree(int tlex_index, int nature, int region, int description, int execution);

/* Mise a jour de la taille a l'execution d'une entree de la table des declarations */
void tdec_maj_taille_exec(int index, int taille);

/* Recuperation de la taille a l'execution d'une entree de la table des declarations */
int tdec_recupere_taille_exec(int index);

/* Recuperation de l'index d'une entree dans la table des declarrations a partir de son index lexicographique */
int tdec_trouve_index(int tlex_index, pile PREG);

/* Affichage de la table des declarations */
void tdec_affiche();

#endif