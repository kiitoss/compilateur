#ifndef UTILS_H
#define UTILS_H

#include "../inc/global.h"
#include "../inc/tdec.h"
#include "../inc/tlex.h"
#include "../inc/trep.h"

typedef struct {
    int taille;
    int trep_index_type;
    int trep_index_nb_dimensions;
    int tdec_index;
} global_tableau;

/* Inisitalisation des differentes tables */
void init_tables();

/* * Mise a jour de l'index global de la table lexicographique */
void maj_tlex_index(int tlex_index);

/* Mise a jour du numero global de region */
void maj_num_region(int num_region);

/* Insertion d'un nouveau tableau dans les differentes tables */
void debut_nouveau_tableau();

/*
 * Insertion d'une nouvelle dimension d'un tableau dans les differentes tables
 * et mise a jour du tableau parent dans les differentes tables
 */
void nouvelle_dimension(int borne_min, int borne_max);

/* Mise a jour du type et de la taille du tableau dans les differentes tables */
void fin_nouveau_tableau(int tdec_index_type);

#endif