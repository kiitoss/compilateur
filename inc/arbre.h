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
#define TAILLE_ESPACE 3

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
#define A_LISTE_DIM    9
#define A_DIM          10
#define A_CHAMP        11
#define A_TYPE         12
#define A_TAB          13
#define A_DECL_VAR     14
#define A_DECL_FONC    15
#define A_DECL_PROC    16
#define A_PARAM        17
#define A_AFFICHE      18
#define A_RETOURNE     19
#define A_ECRIRE       20
#define A_LIRE         21
#define A_FORMAT       22
#define A_TANT_QUE     23
#define A_LISTE_DECL   24
#define A_LISTE_INSTR  25
#define A_LISTE_PARAM  26
#define A_LISTE_CHAMPS 27
#define A_STRUCT       28
#define A_LISTE_ARGS   29
#define A_TYPE_B       30

/* Structure d'un arbre */
typedef struct noeud {
    int nature;                 // nature du noeud
    int valeur;                 // valeur du noeud
    struct noeud *fils_gauche;  // fils gauche du noeud
    struct noeud *frere_droit;  // frere droit du noeud
} noeud;

typedef struct noeud *arbre;

/* Creation d'un arbre vide */
arbre arbre_creer_arbre_vide();

/* Retourne 1 si l'arbre est vide, 0 sinon */
int arbre_est_vide(arbre a);

/* Creation d'un nouveau noeud */
arbre arbre_creer_noeud(int nature, int valeur);

/* Creation d'un arbre */
arbre arbre_creer_arbre(int nature, int valeur, arbre fils_gauche, arbre frere_droit);

/* Concatenation de l'arbre pere et de l'arbre pere */
arbre arbre_concat_pere_fils(arbre pere, arbre fils);

/* Concatenation de l'arbre pere et de l'arbre frere */
arbre arbre_concat_pere_frere(arbre pere, arbre frere);

/* Affichage de l’arbre */
void arbre_affiche(arbre a);

#endif