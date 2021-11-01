#include "global.h"

#ifndef TREP_H
#define TREP_H

#define TREP_TAILLE_MAX TLEX_TAILLE_MAX * 3

/* Structure */
struct trep_entree_structure_champ {
    int type;
    int num_lex;
    int exec;
    struct trep_entree_structure_champ *suivant;
};

struct trep_entree_structure {
    int nb_champs;
    struct trep_entree_structure_champ *champ;
};

/* Tableau */
struct trep_entree_tableau_dimension {
  int borne_inf;
  int borne_sup;
  struct trep_entree_tableau_dimension *suivant;
};

struct trep_entree_tableau {
  int type;
  int nb_dimensions;
  struct trep_entree_tableau_dimension *dimension;
};

/* Fonction / Procedure */
struct trep_entree_fonction_param {
  int num_lex;
  int type;
  struct trep_entree_fonction_param *suivant;
};

struct trep_entree_fonction {
  int type;
  int nb_parametres;
  struct trep_entree_fonction_param *parametre;
};

/* Entree */
union entree {
    struct trep_entree_structure structure;
    struct trep_entree_tableau tableau;
    struct trep_entree_fonction fonction;
};

/* la structure d'une entrée dans la table des declarations */
struct trep_entree {
    int nature;
    union entree data;
};

struct trep_entree trep[TREP_TAILLE_MAX];

/* Affiche la table des representations */
void init_trep();

/* Affiche la table des representations */
void trep_affiche();

#endif