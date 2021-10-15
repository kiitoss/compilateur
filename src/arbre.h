#ifndef FSEM_H

#define FSEM_H

/* la structure d’arbre necessaire */
struct arbre
{
  int noeud;
  struct arbre *gauche;
  struct arbre *droit;
};

typedef struct arbre *arbre;

/* cree un noeud dont le champ boeud prend la valeur val
et le fils gauche et le frere droit la valeur NULL */
arbre creerNoeud(int val);

/* retourne toujours la valeur NULL */
arbre arbreVide();

/* Concatene a2 au frere droit de a1 */
arbre concatPereFrere(arbre a1, arbre a2);

/* Concatene a2 au fils gauche de a1 */
arbre concatPereFils(arbre t1, arbre a2);

/* affiche l’arbre t */
void afficheArbre(arbre a);

#endif