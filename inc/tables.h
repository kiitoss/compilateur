#ifndef TABLES_H
#define TABLES_H

#include <stdio.h>
#include <stdlib.h>

#include "tdec.h"
#include "tlex.h"
#include "treg.h"
#include "trep.h"

/* Taille maximale de la table lexicographique */
#define TLEX_TMAX 500

/* Valeur NULL par defaut */
#define VALEUR_NULL -1

/* Liste des natures */
#define TYPE_B 0
#define TYPE_S 1
#define TYPE_T 2
#define VAR    3
#define PROC   4
#define FONC   5

/* Inititalisation des differentes tables */
void init_tables();

/* Affichage des differentes tables */
void affiche_tables();

/* Sauvegarde des tables */
void sauvegarde_tables(FILE *f);

#endif