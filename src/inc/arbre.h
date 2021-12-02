#ifndef ARBRE_H

#define ARBRE_H

#define A_NONE 0
#define A_AFFECT 1
#define A_IDF 2
#define A_CONDITION 3
#define A_SI 4
#define A_ALORS 5
#define A_SINON 6
#define A_CORPS 7
#define A_DECL_TYPE 8
#define A_LISTE_DIM 9
#define A_DIM 10
#define A_CHAMP 11
#define A_TYPE 12
#define A_TAB 13
#define A_DECL_VAR 14
#define A_DECL_FONC 15
#define A_DECL_PROC 16
#define A_PARAM 17
#define A_AFFICHE 18
#define A_RETOURNE 19
#define A_ECRIRE 20
#define A_LIRE 21
#define A_FORMAT 22
#define A_TANT_QUE 23
#define A_LISTE_DECL 24
#define A_LISTE_INSTR 25
#define A_LISTE_PARAM 26
#define A_LISTE_CHAMPS 27
#define A_STRUCT 28
#define A_LISTE_ARGS 29

/* la structure d'arbre necessaire */
typedef struct noeud {
    int nature;
    int valeur;
    struct noeud *fils_gauche;
    struct noeud *frere_droit;

} noeud;

typedef struct noeud *arbre;

/* renvoie un arbre vide */
arbre creer_arbre_vide();

/* teste si un arbre est vide */
int est_vide(arbre a);

/* cree un noeud dont le champ boeud prend la valeur val
   et le fils gauche et le frere droit la valeur NULL */
arbre creer_noeud(int nature, int valeur);

/* Creer un arbre */
arbre creer_arbre(int nature, int valeur, arbre fils_gauche, arbre frere_droit);

/* Concatene l'arbre fils et l'arbre pere */
arbre concat_pere_fils(arbre pere, arbre fils);

/* Concatene l'arbre fils et l'arbre frere */
arbre concat_pere_frere(arbre pere, arbre frere);

/* Affichage de l’arbre */
void afficher_arbre(arbre a);

#endif