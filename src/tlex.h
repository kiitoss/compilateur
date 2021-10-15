/* la structure de la table lexicographique */
struct entry
{
  int size;
  char *lexeme;
  struct entry *next;
};

typedef struct entry *tlex;