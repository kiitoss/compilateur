#include "../../inc/arbre.h"

/*
 * Retourne sous forme de chaine de caractere la nature d'un noeud
 */
static char *arbre_recupere_nature_str(int nature) {
    switch (nature) {
        case A_NONE:
            return "null";
        case A_AFFECT:
            return "affectation";
        case A_IDF:
            return "identificateur";
        case A_CONDITION:
            return "condition";
        case A_SI:
            return "si";
        case A_ALORS:
            return "alors";
        case A_SINON:
            return "sinon";
        case A_CORPS:
            return "corps";
        case A_DECL_TYPE:
            return "declaration type";
        case A_LISTE_DIM:
            return "liste dimensions";
        case A_DIM:
            return "dimension";
        case A_CHAMP:
            return "champ";
        case A_TYPE:
            return "type";
        case A_TAB:
            return "tableau";
        case A_DECL_VAR:
            return "variable";
        case A_DECL_FONC:
            return "fonction";
        case A_DECL_PROC:
            return "procedure";
        case A_PARAM:
            return "parametre";
        case A_AFFICHE:
            return "affiche";
        case A_RETOURNE:
            return "retourne";
        case A_ECRIRE:
            return "ecrire";
        case A_LIRE:
            return "lire";
        case A_FORMAT:
            return "format";
        case A_TANT_QUE:
            return "tant que";
        case A_LISTE_DECL:
            return "liste declarations";
        case A_LISTE_INSTR:
            return "liste instructions";
        case A_LISTE_PARAM:
            return "liste parametres";
        case A_LISTE_CHAMPS:
            return "liste champs";
        case A_STRUCT:
            return "sructure";
        case A_LISTE_ARGS:
            return "liste arguments";
        default:
            return "inconnu";
    }
}

/*
 * Creation d'un arbre vide
 */
arbre arbre_creer_arbre_vide() { return NULL; }

/*
 * Retourne 1 si l'arbre est vide, 0 sinon
 */
int arbre_est_vide(arbre a) { return a == arbre_creer_arbre_vide(); }

/*
 * Creation d'un nouveau noeud
 */
arbre arbre_creer_noeud(int nature, int valeur) {
    arbre a        = malloc(sizeof(arbre));
    a->nature      = nature;
    a->valeur      = valeur;
    a->fils_gauche = arbre_creer_arbre_vide();
    a->frere_droit = arbre_creer_arbre_vide();

    return a;
}

/*
 * Creation d'un arbre
 */
arbre arbre_creer_arbre(int nature, int valeur, arbre fils_gauche, arbre frere_droit) {
    arbre a = arbre_creer_noeud(nature, valeur);

    a->fils_gauche = fils_gauche;
    a->frere_droit = frere_droit;

    return a;
}

/*
 * Concatenation de l'arbre pere et de l'arbre pere
 */
arbre arbre_concat_pere_fils(arbre pere, arbre fils) {
    pere->fils_gauche = fils;
    return pere;
}

/*
 * Concatenation de l'arbre pere et de l'arbre frere
 */
arbre arbre_concat_pere_frere(arbre pere, arbre frere) {
    if (!arbre_est_vide(pere->frere_droit)) {
        pere->frere_droit = arbre_concat_pere_frere(pere->frere_droit, frere);
    } else {
        pere->frere_droit = frere;
    }
    return pere;
}

/* Fonction auxiliaire d'affichage de l'arbre */
static void arbre_affiche_bis(arbre a, int espace) {
    /* afffichage du noeud vide */
    if (arbre_est_vide(a)) {
        if (AFFICHE_NOEUD_VIDE) {
            printf("\n");
            for (int i = 0; i < espace * TAILLE_ESPACE; i++) printf(" ");
            printf(".NULL\n");
        }
        return;
    }

    /* afffichage de l'espace */
    for (int i = 0; i < espace * TAILLE_ESPACE; i++) printf(" ");

    /* affichage du noeud */
    printf(".nature::%s", arbre_recupere_nature_str(a->nature));
    if (a->valeur != VALEUR_NULL) {
        printf(" // valeur::%d", a->valeur);
    }
    printf("\n");

    /* appel recursif de la fonction d'affichage des fils */
    espace += TAILLE_ESPACE;
    arbre_affiche_bis(a->fils_gauche, espace);

    espace -= TAILLE_ESPACE;
    arbre_affiche_bis(a->frere_droit, espace);
}

/* Affichage de l’arbre */
void arbre_affiche(arbre a) { arbre_affiche_bis(a, 0); }