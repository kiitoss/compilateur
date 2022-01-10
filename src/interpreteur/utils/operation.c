#include "../../../inc/interpreteur.h"

/*
 * Retourne une cellule integrant la valeur d'une operation
 * type_retour : type de la cellule
 * Cast automatique d'un entier en reel ou d'un reel en entier
 */
cellule resout_operation(arbre a, cellule gauche, cellule droite, int type_retour) {
    /* initialisation de la cellule */
    cellule c = cellule_null();

    /* recuperation de la nature de l'operation */
    int nature = arbre_recupere_nature(a);

    /* verification de la validite de la division */
    if (nature == A_DIV && ((type_retour == 0 && droite.entier == 0) || (type_retour == 1 && droite.reel == 0))) {
        fprintf(stderr, "Division par 0 interdite !\n");
        exit(EXIT_FAILURE);
    }

    /* mise a jour de la cellule avec le resultat de l'operation */
    if (type_retour == 0) {
        switch (nature) {
            case A_PLUS:
                c.entier = gauche.entier + droite.entier;
                return c;
            case A_MOINS:
                c.entier = gauche.entier - droite.entier;
                return c;
            case A_MULT:
                c.entier = gauche.entier * droite.entier;
                return c;
            case A_DIV:
                c.entier = gauche.entier / droite.entier;
                return c;
            default:
                return c;
        }
    }

    if (type_retour == 1) {
        switch (nature) {
            case A_PLUS:
                c.reel = gauche.reel + droite.reel;
                return c;
            case A_MOINS:
                c.reel = gauche.reel - droite.reel;
                return c;
            case A_MULT:
                c.reel = gauche.reel * droite.reel;
                return c;
            case A_DIV:
                c.reel = gauche.reel / droite.reel;
                return c;
            default:
                return c;
        }
    }

    return c;
}
