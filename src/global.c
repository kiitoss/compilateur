#include "inc/global.h"

#include <stdio.h>
#include <stdlib.h>

int taille_tlex = 0;
int taille_trep = 0;

/* Affiche la nature d'une entree de la table des declarations ou des representations */
void affiche_nature_declaration(int nature) {
    switch (nature) {
        case NATURE_STRUCTURE:
            printf("structure");
            break;
        case NATURE_TABLEAU:
            printf("tableau");
            break;
        case NATURE_VARIABLE:
            printf("variable");
            break;
        case NATURE_PARAMETRE:
            printf("parametre");
            break;
        case NATURE_PROCEDURE:
            printf("procedure");
            break;
        case NATURE_FONCTION:
            printf("fonction");
            break;
        default:
            printf("null/inconnu");
            break;
    }
}