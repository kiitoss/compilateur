#include <stdio.h>
#include <stdlib.h>
#include "fsem.h"

#define SPACE_SIZE 5

/* cree un noeud dont le champ node prend la valeur value
et le fils gauche et le frere droit la valeur NULL */
tree create_node(int value)
{
  tree t = malloc(sizeof(tree));
  t->node = value;
  return t;
}

/* retourne toujours la valeur NULL */
tree finish()
{
  return NULL;
}

/* Concatene t2 au frere droit de t1 */
tree concat_father_brother(tree t1, tree t2)
{
  return finish();
}

/* Concatene t2 au fils gauche de t1 */
tree concat_father_son(tree t1, tree t2)
{
  return finish();
}

static void traverse_bis(tree t, int space)
{
  int i;
  if (t == finish())
    return;

  space += SPACE_SIZE;

  traverse_bis(t->right, space);

  printf("\n");
  for (i = SPACE_SIZE; i < space; i++)
    printf(" ");
  printf("%d\n", t->node);

  traverse_bis(t->left, space);
}

/* affiche l’arbre t */
void traverse(tree t)
{
  traverse_bis(t, 0);
}

/* range la chaine s dans la table lexicographique si elle n’y est pas
deja ; retourne le numero lexicographique de s */
int numlex_of_string(char *s)
{
  printf("%s", s);
  return 0;
}

/* initialise la table lexicographique */
void init_tab_lex()
{
}
