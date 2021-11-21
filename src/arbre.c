#include "inc/arbre.h"

#include <stdio.h>
#include <stdlib.h>

#include "inc/global.h"

#define TAILLE_ESPACE 3
#define AFFICHE_NULL 0

static void affiche_nature(int nature) {
    switch (nature) {
        case A_NONE:
            printf("null");
            break;
        case A_AFFECT:
            printf("affectation");
            break;
        case A_IDF:
            printf("identificateur");
            break;
        case A_CONDITION:
            printf("condition");
            break;
        case A_SI:
            printf("si");
            break;
        case A_ALORS:
            printf("alors");
            break;
        case A_SINON:
            printf("sinon");
            break;
        case A_CORPS:
            printf("corps");
            break;
        case A_DECL_TYPE:
            printf("declaration type");
            break;
        case A_LISTE_DIM:
            printf("liste dimensions");
            break;
        case A_DIM:
            printf("dimension");
            break;
        case A_CHAMP:
            printf("champ");
            break;
        case A_TYPE:
            printf("type");
            break;
        case A_TAB:
            printf("tableau");
            break;
        case A_DECL_VAR:
            printf("variable");
            break;
        case A_DECL_FONC:
            printf("fonction");
            break;
        case A_DECL_PROC:
            printf("procedure");
            break;
        case A_PARAM:
            printf("parametre");
            break;
        case A_AFFICHE:
            printf("affiche");
            break;
        case A_RETOURNE:
            printf("retourne");
            break;
        case A_ECRIRE:
            printf("ecrire");
            break;
        case A_LIRE:
            printf("lire");
            break;
        case A_FORMAT:
            printf("format");
            break;
        case A_TANT_QUE:
            printf("tant que");
            break;
        case A_LISTE_DECL:
            printf("liste declarations");
            break;
        case A_LISTE_INSTR:
            printf("liste instructions");
            break;
        case A_LISTE_PARAM:
            printf("liste parametres");
            break;
        case A_LISTE_CHAMPS:
            printf("liste champs");
            break;
        case A_STRUCT:
            printf("sructure");
            break;
        default:
            printf("inconnu");
    }
}

/*Retourne un arbre vide*/
arbre creer_arbre_vide() { return NULL; }

/* Fonction test: si l'arbre est vide */
int est_vide(arbre a) {
    if (a == creer_arbre_vide()) return 1;
    return 0;
}

/* cree un noeud dont le champ noeud prend la valeur val
et le fils gauche et le frere droit la valeur NULL */
arbre creer_noeud(int nature, int valeur) {
    arbre a = malloc(sizeof(arbre));
    a->nature = nature;
    a->valeur = valeur;
    a->fils_gauche = creer_arbre_vide();
    a->frere_droit = creer_arbre_vide();

    return a;
}

/* Creer un arbre dont le champ noeud prend la valeur "valeur" */
arbre creer_arbre(int nature, int valeur, arbre fils_gauche,
                  arbre frere_droit) {
    arbre a = creer_noeud(nature, valeur);

    a->fils_gauche = fils_gauche;
    a->frere_droit = frere_droit;

    return a;
}

/* Concatene l'arbre fils et l'arbre pere */
arbre concat_pere_fils(arbre pere, arbre fils) {
    pere->fils_gauche = fils;
    return pere;
}

/* Concatene l'arbre fils et l'arbre frere */
arbre concat_pere_frere(arbre pere, arbre frere) {
    if (!est_vide(pere->frere_droit)) {
        pere->frere_droit = concat_pere_frere(pere->frere_droit, frere);
    } else {
        pere->frere_droit = frere;
    }
    return pere;
}

/* Fonction auxiliaire d'affichage de l'arbre */
static void afficher_arbre_bis(arbre a, int espace) {
    int i;
    if (est_vide(a)) {
        if (AFFICHE_NULL) {
            printf("\n");
            for (i = 0; i < espace * TAILLE_ESPACE; i++) printf(" ");
            printf(".NULL\n");
        }
        return;
    }

    for (i = 0; i < espace * TAILLE_ESPACE; i++) {
        printf(" ");
    }
    printf(".nature::");
    affiche_nature(a->nature);
    if (a->valeur != VALEUR_NULL) {
        printf(" // valeur::%d", a->valeur);
    }
    printf("\n");

    espace += TAILLE_ESPACE;
    afficher_arbre_bis(a->fils_gauche, espace);

    espace -= TAILLE_ESPACE;
    afficher_arbre_bis(a->frere_droit, espace);
}

/* Affiche l’arbre a */
void afficher_arbre(arbre a) { afficher_arbre_bis(a, 0); }