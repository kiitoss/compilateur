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

/* Met à jour l'index global de la table des representations sur lequelle le travail s'effectue */
void set_index_global_trep(int index);

/* Retourne l'index global de la table des representations sur lequelle le travail s'effectue */
int get_global_index_trep();

/* Remet à zero l'index global de la table des representations sur laquelle le travail s'effectue */
void reinitialise_index_global_trep();

/* Retourne 1 si l'index global de la table des representations est null, 0 sinon */
int est_null_index_global_trep();

/* Insere un nouveau champ dans la liste de champs d'une structure */
void trep_ajoute_structure_champ(int index, int type, int num_lex, int exec);

/* Insere une nouvelle dimension dans la liste de dimensions d'un tableau */
void trep_ajoute_tableau_dimension(int index, int borne_inf, int borne_sup);

/* Insere un nouveau parametre dans la liste de parametres d'une fonction ou d'une procedure */
void trep_ajoute_fonction_param(int index, int num_lex, int type);

/* Affiche la table des representations */
void init_trep();

/* Cree une nouvelle entree dans le tableau des representations */
int trep_nouvelle_entree(int nature);

/* Affiche la table des representations */
void trep_affiche();

/* Libère la mémoire allouée au tableau des representations */
void trep_free();

#endif