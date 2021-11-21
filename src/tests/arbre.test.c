#include "../inc/arbre.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    // arbre d = creer_arbre(0, 1, creer_arbre(0, 2, creer_noeud(0, 3), creer_noeud(0, 4)),
    //                     creer_arbre(0, 5, creer_noeud(0, 6), creer_noeud(0, 7)));
    // afficheArbre(d);

    arbre d = creer_arbre(0, 1, creer_arbre(0, 2, creer_noeud(0, 4), creer_noeud(0, 5)), creer_noeud(0, 3));

    arbre a = creer_arbre(0, 8, creer_noeud(0, 0), creer_noeud(0, 0));
    d = concat_pere_fils(d, a);

    afficher_arbre(d);

    return EXIT_SUCCESS;
}