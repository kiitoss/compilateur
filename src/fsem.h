#ifndef FSEM_H

#define FSEM_H

/* la structure d’arbre necessaire */
struct tree
{
  int node;
  struct tree *left;
  struct tree *right;
};

typedef struct tree *tree;

/* cree un noeud dont le champ node prend la valeur value
et le fils gauche et le frere droit la valeur NULL */
tree create_node(int value);

/* retourne toujours la valeur NULL */
tree finish();

/* Concatene t2 au frere droit de t1 */
tree concat_father_brother(tree t1, tree t2);

/* Concatene t2 au fils gauche de t1 */
tree concat_father_son(tree t1, tree t2);

/* affiche l’arbre t */
void traverse(tree t);

/* range la chaine s dans la table lexicographique si elle n’y est pas
deja ; retourne le numero lexicographique de s */
int numlex_of_string(char *s);

/* initialise la table lexicographique */
void init_tab_lex();

#endif