#include "inc/arbre.h"

#include <stdio.h>
#include <stdlib.h>

#define TAILLE_ESPACE 5

/*Retourne un arbre vide*/
arbre creeArbreVide() { return NULL; }

/* Fonction test: si l'arbre est vide */
int estVide(arbre a) {
    if (a == creeArbreVide()) return 1;
    return 0;
}

/* cree un noeud dont le champ noeud prend la valeur val
et le fils gauche et le frere droit la valeur NULL */
arbre creeNoeud(int valeur) {
    arbre a = malloc(sizeof(arbre));

    a->valeur = valeur;
    a->filsGauche = creeArbreVide();
    a->filsDroit = creeArbreVide();

    return a;
}

/* Creer un arbre dont le champ noeud prend la valeur "valeur" */
arbre creeArbre(int valeur, arbre filsGauche, arbre filsDroit) {
    arbre a = creeNoeud(valeur);

    a->filsGauche = filsGauche;
    a->filsDroit = filsDroit;

    return a;
}

/* Concatene filsDroit au frere droit de a */
arbre insereFilsDroit(arbre a, arbre filsDroit) {
    if (estVide(a))
        return filsDroit;
    else
        a->filsDroit = insereFilsDroit(a->filsDroit, filsDroit);

    return a;
}

/* Concatene filsGauche au fils gauche de a */
arbre insereFilsGauche(arbre a, arbre filsGauche) {
    if (estVide(a))
        return filsGauche;
    else
        a->filsGauche = insereFilsGauche(a->filsGauche, filsGauche);

    return a;
}

/* Fonction auxiliaire d'affichage de l'arbre */
static void afficheArbreBis(arbre a, int espace) {
    int i;
    if (estVide(a)) {
        printf("\n");
        for (i = TAILLE_ESPACE; i < espace; i++) printf("  ");
        printf(".NULL\n");
        return;
    }
    espace += TAILLE_ESPACE;

    afficheArbreBis(a->filsDroit, espace);
    printf("\n");
    for (i = TAILLE_ESPACE; i < espace; i++) {
        printf(" ");
    }
    printf(".%d\n", a->valeur);

    afficheArbreBis(a->filsGauche, espace);
}

/* Affiche l’arbre a */
void afficheArbre(arbre a) { afficheArbreBis(a, 0); }