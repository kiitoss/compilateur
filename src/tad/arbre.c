#include "../../inc/arbre.h"

/*
 * Retourne sous forme de chaine de caractere la nature d'un noeud
 */
static char *arbre_recupere_nature_str(int nature) {
    switch (nature) {
        case A_NONE:
            return "null";
        case A_AFFECT:
            return "affect";
        case A_IDF:
            return "idf";
        case A_CONDITION:
            return "cond";
        case A_SI:
            return "si";
        case A_ALORS:
            return "alors";
        case A_SINON:
            return "sinon";
        case A_CORPS:
            return "corps";
        case A_DECL_TYPE:
            return "decl_type";
        case A_LISTE_DIM:
            return "liste_dim";
        case A_DIM:
            return "dim";
        case A_CHAMP:
            return "champ";
        case A_TYPE:
            return "type";
        case A_TAB:
            return "tab";
        case A_DECL_VAR:
            return "decl_var";
        case A_DECL_FONC:
            return "fonction";
        case A_DECL_PROC:
            return "proc";
        case A_PARAM:
            return "param";
        case A_AFFICHE:
            return "affiche";
        case A_RETOURNE:
            return "retourne";
        case A_TANT_QUE:
            return "tant que";
        case A_LISTE_DECL:
            return "liste_decl";
        case A_LISTE_INSTR:
            return "liste_inst";
        case A_LISTE_PARAM:
            return "listeparam";
        case A_LISTE_CHAMPS:
            return "liste_champs";
        case A_STRUCT:
            return "sruct";
        case A_LISTE_ARGS:
            return "liste_args";
        case A_EXPR_BOOL:
            return "expr_bool";
        case A_NON:
            return "negation";
        case A_ET:
            return "et";
        case A_TEST_ARITH:
            return "test_arith";
        case A_DOUBLE_EGAL:
            return "double_egal";
        case A_SUP:
            return "sup";
        case A_INF:
            return "inf";
        case A_SUP_EGAL:
            return "sup_egal";
        case A_INF_EGAL:
            return "inf_egal";

        case A_RESULTAT_RET:
            return "retourne_res";
        case A_ARG:
            return "arg";
        case A_VAR:
            return "var";
        case A_APPEL:
            return "appel";
        case A_EXPR_ARITH:
            return "expr_arith";
        case A_PLUS:
            return "plus";
        case A_MOINS:
            return "moins";
        case A_MULT:
            return "mult";
        case A_DIV:
            return "div";
        case A_OU:
            return "ou";
        case A_LECTURE_TAB:
            return "lecture_tab";
        case A_ECRIT:
            return "ecrit";
        case A_ENTIER:
            return "entier";
        case A_REEL:
            return "reel";
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
arbre arbre_creer_noeud(int nature, int valeur_1, int valeur_2) {
    arbre a        = malloc(sizeof(struct noeud));
    a->nature      = nature;
    a->valeur_1    = valeur_1;
    a->valeur_2    = valeur_2;
    a->fils_gauche = arbre_creer_arbre_vide();
    a->frere_droit = arbre_creer_arbre_vide();

    return a;
}

/*
 * Creation d'un nouveau noeud sans valeur
 */
arbre arbre_creer_noeud_vide(int nature) { return arbre_creer_noeud(nature, VALEUR_NULL, VALEUR_NULL); }

/*
 * Creation d'un arbre
 */
arbre arbre_creer_arbre(int nature, int valeur_1, int valeur_2, arbre fils_gauche, arbre frere_droit) {
    arbre a = arbre_creer_noeud(nature, valeur_1, valeur_2);

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

/*
 * Fonction auxiliaire d'affichage de l'arbre
 */
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
    printf("%s", arbre_recupere_nature_str(a->nature));
    if (a->valeur_1 != VALEUR_NULL || a->valeur_2 != VALEUR_NULL) {
        printf(" [");
        if (a->valeur_1 != VALEUR_NULL) {
            printf("%d", a->valeur_1);
        }
        if (a->valeur_1 != VALEUR_NULL && a->valeur_2 != VALEUR_NULL) {
            printf("][");
        }
        if (a->valeur_2 != VALEUR_NULL) {
            printf("%d", a->valeur_2);
        }
        printf("]");
    }
    printf("\n");

    /* appel recursif de la fonction d'affichage des fils */
    espace += TAILLE_ESPACE;
    arbre_affiche_bis(a->fils_gauche, espace);

    espace -= TAILLE_ESPACE;
    arbre_affiche_bis(a->frere_droit, espace);
}

/*
 * Sauvegarde de l’arbre
 */
void arbre_affiche(arbre a) { arbre_affiche_bis(a, 0); }

/*
 * Fonction auxiliaire de sauvegarde de l'arbre
 */
void arbre_sauvegarde(FILE *f, arbre a, int niveau, int espace) {
    /* ignore les noeud vide */
    if (arbre_est_vide(a)) return;

    /* afffichage de l'espace */
    for (int i = 0; i < espace * TAILLE_ESPACE; i++) fprintf(f, "\t");

    /* affichage du noeud */
    fprintf(f, "%d | %d[%d][%d]\n", niveau, a->nature, a->valeur_1, a->valeur_2);

    /* appel recursif de la fonction de sauvegarde des fils */
    espace += TAILLE_ESPACE;
    arbre_sauvegarde(f, a->fils_gauche, niveau + 1, espace);

    espace -= TAILLE_ESPACE;
    arbre_sauvegarde(f, a->frere_droit, niveau, espace);
}

/*
 * Recupere la nature de l'arbre
 */
int arbre_recupere_nature(arbre a) {
    if (arbre_est_vide(a)) {
        return A_NONE;
    } else {
        return a->nature;
    }
}