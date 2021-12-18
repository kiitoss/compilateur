#ifndef THASH_H
#define THASH_H

#ifndef VALEUR_NULL
#define VALEUR_NULL -1
#endif

#include <stdio.h>

/* Taille maximale de la table de hash-code */
#define THASH_TMAX 32

/* Generation d'un hash a partir d'un lexeme */
int genere_hash(char *lexeme);

/* Initialisation de la table de hash-code avec des valeurs nulles */
void thash_init();

/* Ecriture d'une nouvelle entree dans la table de hash-code */
void thash_ecrit(int index, int valeur);

/* Recuperation d'une entree de la table de hash-code */
int thash_recupere_valeur(int index);

/* Affichage de la table de hash-code */
void thash_affiche();

#endif