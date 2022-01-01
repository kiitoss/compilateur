#ifndef PILE_EXEC_H
#define PILE_EXEC_H

#include <stdio.h>
#include <stdlib.h>

/* Taille maximale de la pile d'execution */
#define PEXEC_TMAX 5000

/* Valeur NULL par defaut */
#ifndef VALEUR_NULL
#define VALEUR_NULL -1
#endif

union cellule {
    int entier;
    float reel;
    char booleen;
    char caractere;
};

typedef union cellule cellule;

typedef cellule pexec[PEXEC_TMAX];

#endif