#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "arbre.h"
#include "pexec.h"
#include "tables.h"

extern int line;

extern int BC;
extern pexec PEXEC;
extern int taille_pexec;
extern pile PREG;
extern pile PFONC;
extern int BC_regions[100];
extern int reg_parentes[100];

/* Execution du programme apres recuperation des donnees */
void execution();

/*
 * Recuperation de l'index d'une variable dans la pile d'execution
 * a partir de son index lexicographique et de son deplacement a l'execution
 */
int pexec_index_variable(int tlex_index, int deplacement_exec);

/* Recuperation de l'index de la cellule concerne par un appel a un tableau dans la pile d'execution */
int pexec_index_tableau(arbre a);

void resout_appel(arbre a);

/*
 * Retourne une cellule integrant la valeur de la variable stockee dans l'arbre
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
cellule resout_variable(arbre a, int type_retour);

/*
 * Retourne une cellule integrant la valeur d'une operation
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
cellule resout_operation(arbre a, cellule gauche, cellule droite, int type_retour);

int resout_expression_booleenne(arbre a);

/*
 * Retourne une cellule integrant la valeur d'une expression
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
cellule resout_expression(arbre a, int type_retour);

/* Execution d'une region = interpretation */
void execute_region(int region);

/* Resolution d'un affichage sur la sortie standard */
void resout_ecriture(arbre a);

/* Resolution d'une lecture sur l'entree standard */
void resout_lecture(arbre a);

/* Resolution d'une affectation */
void resout_affectation(arbre a);

/* Recuperation de l'index de la cellule concerne par un appel a une structure dans la pile d'execution */
int pexec_index_structure(arbre a);

#endif