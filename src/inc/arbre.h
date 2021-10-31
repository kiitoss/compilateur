#ifndef FSEM_H

#define FSEM_H

/* la structure d’arbre necessaire */
typedef struct noeud {
    int valeur;
    int nature;
    struct noeud *filsGauche;
    struct noeud *filsDroit;
} noeud;

typedef struct noeud *arbre;

/* renvoie un arbre vide */
arbre creeArbreVide();

/* teste si un arbre est vide */
int estVide(arbre a);

/* cree un noeud dont le champ boeud prend la valeur val
   et le fils gauche et le frere droit la valeur NULL */
arbre creeNoeud(int valeur);

/* Creer un arbre */
arbre creeArbre(int valeur, arbre filsGauche, arbre filsDroit);

/* Insere un fils gauche*/
arbre insereFilsGauche(arbre a, arbre filsGauche);

/* Insere un fils droit */
arbre insereFilsDroit(arbre a, arbre filsDroit);

/* Affichage de l’arbre */
void afficheArbre(arbre a);

#endif