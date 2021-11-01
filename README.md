# Réalisation d'un compilateur et d'une machine virtuelle pour le langage CPYRR

Ce projet a pour but la création d'un compilateur et d'une machine virtuelle pour le langage CPYRR, un langage procédural à structure de blocs.

## Table des matieres
1. [Conception des programmes LEX et YACC](#p1)
    1. [Le programme LEX](#p11)
    2. [Le programme YACC](#p12)
2. [Création du compilateur (analyses lexicale, synthaxique, sémantique et génération du texte intermédiaire)](#p2)
    1. [Analyse Lexicale et synthaxique](#p21)
    2. [Analyse Sémantique](#p22)
        1. [La table de hash-code](#p221)
        2. [La table lexicographique](#p222)
        3. [La table des declarations](#p223)
3. [Conception de la machine virtuelle](#p3)

## Conception des programmes LEX et YACC <a name="p1"></a>
### Le programme LEX <a name="p11"></a>
Le programme LEX nous sert à faire l'analyse lexicale du fichier source.
Chaque caractère ou chaîne de caractère est analysé puis comparé à une liste de règles.
Lorsqu'une règle est matché, l'action correspondante est effectuée.

Dans le cadre de ce projet de compilateur, on renverra la plupart du temps des "tokens" que le programme YACC pourra ensuite récupérer puis analyser à son tour pour l'analyse synthaxique.
Par exemple, le mot "fonction" renverra le token FONCTION.
En revanche, lorsque l'analyseur lexical reconnaît un entier ([1-9]+[0-9]*) que l'on appelera x, on affectera à **yylval** la valeur atoi(x) puis on retournera le token ENTIER.
```
%{
  #include <stdlib.h>
  #include "y.tab.h" // Lie ensemble le programme LEX et le programme YACC
	void yyerror(char *);
%}

%%
	/* Permet d'ignorer les espaces et les lignes vides */
[ \t\n] ;

  /* Déclaration du token FONCTION */
"fonction" return FONCTION;

  /* Déclaration des nombres */
[1-9]+[0-9]* {
  yylval = atoi(yytext);
  return ENTIER;
}

  /* Déclaration des caractères, servant d'identificateurs (IDF) */
[a-zA-Z] {
  yylval = yytext[0] - 'a';
  return IDF;
}

  /* Cas d'erreur */
. yyerror("Caractère invalide !");
%%

int yywrap(void) {
  return 1;
}
```

Cela permettra à YACC d'avoir accès à la valeur de ce nombre lorsqu'il analysera le comportement synthaxique d'un token ENTIER.


### Le programme YACC <a name="p12"></a>
Le programme YACC nous sert à faire l'analyse synthaxique du fichier source.
A ce stade, l'analyseur lexical a terminé son travail et à renvoyé au programme YACC les tokens correspondants au fichier source.

C'est à ce moment qu'il sera vérifié, par exemple, que la fonction soit bien suivie d'un caractère représentant son nom.

```
%{
  #include <stdio.h>
  #include <stdlib.h>

  int yylex(void);
  void yyerror(char *);
%}

/* Déclaration des token */
%token ENTIER
%token FONCTION
%token IDF

%%
programme: 
| programme corps
;

corps: ENTIER { printf("Vous avez entré l'entier: %d\n", $1); }
| FONCTION IDF { printf("Vous avez défini la fonction %c\n", 'a' + $2); }
;
%%

void yyerror(char *s) {
  fprintf(stderr, "Erreur: %s\n", s);
  exit(-1);
}

int main(void) {
  yyparse();
  return 0;
}
```

On affichera alors un message différent selon que l'utilisateur entre un entier ou une fonction.
Si c'est un entier on affichera sur la sortie standart "Vous avez entré l'entier x", et si c'est une fonction "Vous avez défini la fonction x".
Si le token FONCTION est reconnu mais n'est pas suivit d'un identificateur (IDF, ici un caractère), un message d'erreur de synthaxe s'affichera dans la console.


## Création du compilateur (analyses lexicale, synthaxique, sémantique et génération du texte intermédiaire) <a name="p2"></a>
### Analyse Lexicale et synthaxique <a name="p21"></a>
Les analyses lexicales et synthaxiques se font par l'intermédiaire des programme LEX et YACC vu précedemment.
C'est l'analyse sémantique qui va nous intéresser maintenant.

### Analyse Sémantique <a name="p22"></a>
L'analyse sémantique sert à contrôler la sémantique du programme, par exemple la validité du nombre de paramètres des fonctions.
Cette analyse va se faire grâce à l'utilisation de cinq tables:
- La table de hash-code
- La table lexicographique
- La table des déclarations
- La table de représentations des types et des entêtes de sous-programme
- La table des régions

#### La table de hash-code <a name="p221"></a>
Cette table va associer à un hash-code donné, l'index de sa première occurence dans la table lexicographique.  
Prenons un exemple: le lexeme "maVariable".

Le hash-code du lexème "maVariable" est de 20 ([ascii(m) + ascii(a) + ascii(V) + ... + ascii(e)] % 32).  
Si le lexeme "maVariable" est la première entrée de la table lexicographique, nous aurons thash[20] = 0.  
Si cela avait été la deuxième entrée, nous aurions eu thash[20] = 1.

En somme, thash[hash_val] = index_table_lexico;

Prenons maintenant le lexeme "amVariable" ("a" et "m" sont inversés). Le haschode est encore de 20, or thash[20] a déjà été définie.  
Il est donc inutile de modifier la table de hash-code.

Mais alors comment retrouver les informations du lexeme "amVariable" dans la table lexicographique ?  
Nous allons voir cela plus bas.

#### La table lexicographique <a name="p222"></a>
Cette table stocke pour chaque lexeme ses informations:
- La longueur
- Le lexème
- Un pointeur sur le lexeme suivant de même hashcode

Reprenons le même exemple que plus haut:  
Le lexeme "maVariable" est déclaré, puis le lexeme "amVariable".  
Tous deux ont le même hash-code mais c'est "maVariable" qui est déclaré en premier, et donc stocké dans notre table de hashcode.

Affichage de la table de hashcode:
```
indice/hashcode  |  index table lexico
0                |  -1
1                |  -1
2                |  -1
...              |  -1
19               |  -1
20               |  0
...              |  -1
35               |  -1
```

Et voici la table lexicographique:
```
indice  |  longueur  |  lexeme      |  suivant
0       |  10        |  maVariable  |  1
1       |  10        |  amVariable  |  -1
```

A son insertion dans la table lexicographique, un lexeme n'a pas de "suivant", on lui affecte donc une valeur NULL, ici représenté par la valeur -1.  
Si un autre lexeme de même hascode (par exemple "amariableV") venait à être créé, il faudrait modifier la valeur de suivant de "amVariable" pour qu'elle prenne l'index du nouveua lexeme.

Voici la table de hash-code après plusieurs modifications:
```
indice/hashcode  |  index table lexico
0                |  -1
1                |  2
2                |  -1
...              |  -1
19               |  -1
20               |  0
...              |  -1
32               |  -1
```

Et voici la table lexicographique après plusieurs modifications:
```
indice  |  longueur  |  lexeme      |  suivant
0       |  10        |  maVariable  |  1
1       |  10        |  amVariable  |  4
2       |  1         |  a           |  3
3       |  10        |  monFloat01  |  -1
4       |  10        |  amariableV  |  -1
```

Dans cette nouvelle table lexicographique, il y a 5 entrées, et pourtant, il n'y a que deux hash-code différents:  
hashcode(a) = hashcode(monFloat01)  
hashcode(maVariable) = hashcode(amVariable) = hashcode(amariableV)

Pour obtenir l'indice du lexeme "amariableV", il suffira de faire:
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Inclure ici le fichier contenant la fonction hash_lexeme() et initialisant le tableau thash */
#include "tlex.h"

#define THASH_MAX 32

/* Retourne l'indice d'un lexeme dans la table lexico, -1 sinon */
int get_indice(char *lexeme) {
  int indice = -1;
  int hash_val = hash_lexeme(lexeme);

  /* On initialise i grâce à la table de hashcode */
  /* On passe au i suivant grâce à la colonne "suivant" de la table lexico */
  for (int i = thash[hash_val]; i != -1; i = tlex[i].suivant) {
    if (strcmp(thash[i], lexeme == 0) {
      indice = i;
      break;
    }
  }

  return indice;
}

int main(void) {
  int indice = get_indice("amariableV");
  if (indice == -1) {
    printf("Aucune entrée pour 'amariableV' dans la table.\n");
  } else {
    printf("'amariableV' se trouve en indice %d dans la table lexicographique.\n", indice);
  }
  return EXIT_SUCCESS;
}
```


#### La table des declarations <a name="p223"></a>
La table des declarations sert a à stocker les informations sur un lexeme.
Cette table enregistre les informations suivantes:
- La nature (structure / tableau / variable / parametre / procedure / fonction)
- Un chainage vers le lexeme suivant de même nom
- Le numero de la région
- L'index du lexème dans la table des représentations
- La taille ou le déplacement à l'exécution

## Conception de la machine virtuelle <a name="p3"></a>