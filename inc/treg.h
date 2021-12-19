#ifndef TREG_H
#define TREG_H

#ifndef VALEUR_NULL
#define VALEUR_NULL -1
#endif

#include <stdio.h>

/* Taille maximale de la table des regions */
#define TREG_TMAX 20

/* Structure d'une ligne de la table des regions */
typedef struct {
    int taille;  // taille de la region
    int nis;     // nombre de regions parentes
} treg_entree;

/* Insertion d'une nouvelle entree dans la table des regions */
int treg_nouvelle_entree(int nis);

/* Recuperation de la taille d'une entree de la table des regions */
int treg_recupere_taille(int index);

/* Mise a jour de la taille d'une entree de la table des regions */
void treg_maj_taille(int index, int taille);

#endif