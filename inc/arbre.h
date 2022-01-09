#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>

#ifndef VALEUR_NULL
#define VALEUR_NULL -1
#endif

/* Affichage ou non d'un noeud vide */
#define AFFICHE_NOEUD_VIDE 0
/* Tailel de l'espace inter-noeud */
#define TAILLE_ESPACE 1

/* Liste des natures des noeuds */
#define A_NONE         0
#define A_AFFECT       1
#define A_IDF          2
#define A_CONDITION    3
#define A_SI           4
#define A_ALORS        5
#define A_SINON        6
#define A_CORPS        7
#define A_DECL_TYPE    8
#define A_DECL_VAR     14
#define A_DECL_FONC    15
#define A_DECL_PROC    16
#define A_LISTE_DIM    9
#define A_DIM          10
#define A_CHAMP        11
#define A_TYPE         12
#define A_TAB          13
#define A_PARAM        17
#define A_AFFICHE      18
#define A_RETOURNE     19
#define A_TANT_QUE     23
#define A_LISTE_DECL   24
#define A_LISTE_INSTR  25
#define A_LISTE_PARAM  26
#define A_LISTE_CHAMPS 27
#define A_LISTE_ARGS   29
#define A_STRUCT       28

#define A_EXPR_BOOL    31
#define A_NON          32
#define A_ET           34
#define A_TEST_ARITH   35
#define A_DOUBLE_EGAL  36
#define A_SUP          37
#define A_INF          38
#define A_SUP_EGAL     39
#define A_INF_EGAL     40
#define A_RESULTAT_RET 41
#define A_ARG          43
#define A_VAR          44
#define A_APPEL        45
#define A_EXPR_ARITH   46
#define A_PLUS         47
#define A_MOINS        48
#define A_MULT         49
#define A_DIV          50
#define A_OU           51
#define A_LECTURE_TAB  52
#define A_ECRIT        53
#define A_ENTIER       54
#define A_REEL         55

/* Structure d'un arbre */
typedef struct noeud {
    int nature;                 // nature du noeud
    int valeur_1;               // valeur 1 du noeud
    int valeur_2;               // valeur 2 du noeud
    struct noeud *fils_gauche;  // fils gauche du noeud
    struct noeud *frere_droit;  // frere droit du noeud
} noeud;

typedef struct noeud *arbre;

/* Creation d'un arbre vide */
arbre arbre_creer_arbre_vide();

/* Retourne 1 si l'arbre est vide, 0 sinon */
int arbre_est_vide(arbre a);

/* Creation d'un nouveau noeud */
arbre arbre_creer_noeud(int nature, int valeur_1, int valeur_2);

/* Creation d'un nouveau noeud sans valeur */
arbre arbre_creer_noeud_vide(int nature);

/* Creation d'un arbre */
arbre arbre_creer_arbre(int nature, int valeur_1, int valeur_2, arbre fils_gauche, arbre frere_droit);

/* Concatenation de l'arbre pere et de l'arbre pere */
arbre arbre_concat_pere_fils(arbre pere, arbre fils);

/* Concatenation de l'arbre pere et de l'arbre frere */
arbre arbre_concat_pere_frere(arbre pere, arbre frere);

/* Affichage de l’arbre */
void arbre_affiche(arbre a);

/* Sauvegarde de l’arbre */
void arbre_sauvegarde(FILE *f, arbre a, int niveau, int espace);

/* Recupere la nature de l'arbre */
int arbre_recupere_nature(arbre a);
#endif