#ifndef UTILS_H
#define UTILS_H

#include "global.h"
#include "pile.h"
#include "tdec.h"
#include "tlex.h"
#include "treg.h"
#include "trep.h"

typedef struct {
    int taille;
    int trep_index_type;
    int trep_index_nb_dimensions;
    int tdec_index;
} global_tableau;

typedef struct {
    int trep_index_nb_champs;
    int tdec_index;
    int deplacement_exec;
} global_structure;

/* Inisitalisation des differentes tables */
void init_tables();

/* * Mise a jour de l'index global de la table lexicographique */
void maj_tlex_index(int tlex_index);

/* Insertion d'une nouvelle variable dans TDEC */
void nouvelle_variable(int tlex_index);

/* Insertion d'un nouveau tableau dans les differentes tables */
void debut_nouveau_tableau();

/*
 * Insertion d'une nouvelle dimension d'un tableau dans les differentes tables
 * et mise a jour du tableau parent dans les differentes tables
 */
void nouvelle_dimension(int borne_min, int borne_max);

/* Mise a jour du type et de la taille du tableau dans les differentes tables */
void fin_nouveau_tableau(int tlex_index_type);

/* Insertion d'une nouvelle structure dans les differentes tables */
void debut_nouvelle_structure();

/*
 * Insertion d'un nouveau champ d'une structure dans les differentes tables
 * et mise a jour de la structure parente dans les differentes tables
 */
void nouveau_champ(int tlex_index);

/* Mise a jour de la taille de la structure dans TDEC */
void fin_nouvelle_structure();

#endif