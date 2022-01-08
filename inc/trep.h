#ifndef TREP_H
#define TREP_H

#include "tables.h"

/* Taille maximale de la table des representations */
#define TREP_TMAX 500

/* Initialisation de la table des representations avec des valeurs nulles */
void trep_init();

/* Ecriture d'une nouvelle entree dans la table des representations */
void trep_ecrit(int index, int valeur);

/* Insertion d'une nouvelle entree dans la table des representations */
int trep_nouvelle_entree(int valeur);

/* Mise a jour d'une valeur de la table des representations */
void trep_maj_valeur(int index, int valeur);

/* Recuperation d'une valeur de la table des representations */
int trep_recupere_valeur(int index);

/* Affichage de la table des representations */
void trep_affiche();

/* Sauvegarde de la table des representations */
void trep_sauvegarde(FILE *f);

#endif