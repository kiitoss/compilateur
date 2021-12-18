#ifndef TDEC_H
#define TDEC_H

#include "global.h"

/* Taille maximale de la table des declarations */
/* = TLEX_TMAX + zone de debordement */
#define TDEC_TMAX 5000
#define t         5

/* Liste des natures */
#define TYPE_B 0
#define TYPE_S 1
#define TYPE_T 2
#define VAR    3
#define PROC   4
#define FONC   5

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
void tdec_nouvelle_entree(int tlex_index, int nature, int region, int description, int execution);

/* Affichage de la table des declarations */
void tdec_affiche();

#endif