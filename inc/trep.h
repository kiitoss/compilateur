#ifndef TREP_H
#define TREP_H

#include "global.h"

/* Taille maximale de la table des representations */
#define TREP_TMAX 500

/* Initialisation de la table des representations avec des valeurs nulles */
void trep_init();

/* Insertion d'une nouvelle entree dans la table des representations */
int trep_nouvelle_entree(int valeur);

/* Affichage de la table des representations */
void trep_affiche();

#endif