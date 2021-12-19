#ifndef UTILS_H
#define UTILS_H

#include "global.h"
#include "pile.h"
#include "tdec.h"
#include "tlex.h"
#include "treg.h"
#include "trep.h"

/* Structure d'une variable globale pour stocker les information d'un tableau */
typedef struct {
    int taille;                    // la taille du tableau (si chaque element du tableau a une taille de 1)
    int trep_index_type;           // index dans la table des representations du type du tableau
    int trep_index_nb_dimensions;  // index dans la table des representations du nb de dimensions du tableau
    int tdec_index;                // index dans la table des declarations du tableau
} global_tableau;

/* Structure d'une variable globale pour stocker les information d'une structure */
typedef struct {
    int trep_index_nb_champs;  // index dans la table des representations du nb de champs de la structure
    int tdec_index;            // index dans la table des declarations de la structure
    int deplacement_exec;      // deplacement a l'execution a l'interieur de la structure
} global_structure;

/* Index dans la table lexicographique */
extern int global_tlex_index;
/* Pile des regions */
extern pile PREG;

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