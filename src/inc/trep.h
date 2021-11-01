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
typedef struct trep_entree_structure_champ structure_champ;

struct trep_entree_structure {
    int nb_champs;
    structure_champ *champ;
};

/* Tableau */
struct trep_entree_tableau_dimension {
  int borne_inf;
  int borne_sup;
  struct trep_entree_tableau_dimension *suivant;
};
typedef struct trep_entree_tableau_dimension tableau_dimension;

struct trep_entree_tableau {
  int type;
  int nb_dimensions;
  tableau_dimension *dimension;
};

/* Fonction / Procedure */
struct trep_entree_fonction_param {
  int num_lex;
  int type;
  struct trep_entree_fonction_param *suivant;
};
typedef struct trep_entree_fonction_param fonction_param;

struct trep_entree_fonction {
  int type;
  int nb_parametres;
  fonction_param *parametre;
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

/* Insere un nouveau champ dans la liste de champs d'une structure */
void trep_ajoute_structure_champ(int index, structure_champ *champ);

/* Insere une nouvelle dimension dans la liste de dimensions d'un tableau */
void trep_ajoute_tableau_dimension(int index, tableau_dimension *dimension);

/* Insere un nouveau parametre dans la liste de parametres d'une fonction ou d'une procedure */
void trep_ajoute_fonction_param(int index, fonction_param *parametre);

/* Affiche la table des representations */
void init_trep();

/* Cree une nouvelle entree dans le tableau des representations */
int trep_nouvelle_entree(int nature);

/* Affiche la table des representations */
void trep_affiche();

/* Libère la mémoire allouée au tableau des representations */
void trep_free();

#endif