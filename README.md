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
            1. [Explication](#p2211)
            2. [Intégration](#p2212)
        2. [La table lexicographique](#p222)
            1. [Explication](#p2221)
            2. [Intégration](#p2222)
        3. [La table des declarations](#p223)
            1. [Explication](#p2231)
            2. [Intégration](#p2232)
        4. [La table des représentations des types et des entêtes de sous-programmes](#p224)
            1. [Explication](#p2241)
            2. [Intégration](#p2242)
        5. [La table des régions](#p225)
            1. [Explication](#p2251)
            2. [Intégration](#p2252)
    3. [Création de l'arbre](#p23)
        1. [Structure](#p231)
        2. [Fonctions](#p232)
        1. [Intégration](#p233)
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
##### Explication <a name="p2211"></a>
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

##### Intégration <a name="p2212"></a>
L'intégration de la table de hashcode se fait directement dans la fonction main du fichier YACC
```c
int main(void) {
  init_thash();

  yyparse();

  return 0;
}
```
Son initialiser va intégrer une valeur nulle définie préalablement à chaque élément du tableau de hashcode.
Une fois ce tableau initialiser, il sera possible de s'en servir avec la table lexicograpahique.

#### La table lexicographique <a name="p222"></a>
##### Explication <a name="p2221"></a>
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

#define THASH_TAILLE_MAX 32

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

##### Intégration <a name="p2222"></a>
La table lexicographique n'a pas besoin d'être initialisée.  
C'est le programme LEX qui va insérer de nouvelles entrées dans la table à chaque nouveau lexeme rencontré.
```
{letter}+({letter}|{digit}|"_")*	{
  yylval = tlex_insere(yytext);
  return IDF;
}
```


#### La table des declarations <a name="p223"></a>
##### Explication <a name="p2231"></a>
La table des declarations sert a à stocker les informations sur un lexeme.
Cette table enregistre les informations suivantes:
- La nature (structure / tableau / variable / parametre / procedure / fonction)
- Un chainage vers le lexeme suivant de même nom
- Le numero de la région
- L'index du lexème dans la table des représentations
- La taille ou le déplacement à l'exécution

##### Intégration <a name="p2232"></a>
La table des déclarations doit être initialisée dans la fonction main du fichier YACC.  
Cette initialisation va permettre de remplir les champs NATURE de la table avec une valeur définie comme null.  
Nous aurons donc:
```c
int main(void) {
  init_thash();
  init_tdec();

  yyparse();

  return 0;
}
```

Ensuite, il faudra remplir cette table, cela se fera dans les règles YACC.  
Par exemple, à chaque nouvelle fonction, on souhaitera insérer au bon index les informations correspondant à la nature et à la région.
```
declaration_fonction:	FONCTION IDF liste_parametres RETOURNE type_simple corps    { tdec_insere($2, NATURE_FONCTION, region); }
```

#### La table des représentations des types et des entêtes de sous-programmes <a name="p224"></a>
##### Explication <a name="p2241"></a>
La table des représentations sert à stocker des informations supplémentaires, dépendantes de la nature de l'objet.
- Pour les structures
  - Nombre de champs de la structure
  - Pour chaque champ:
    - Son type
    - Son numéro lexicographique
    - Le déplacement à l'exécution
- Pour les tableau
  - Le type des éléments
  - Le nombre de dimensions du tableau
  - Pour chaque dimension
    - Sa borne inférieure
    - Sa borne supérieure
- Pour les fonctions et les procédures
  - Le type de la valeur retounrnée (inutile pour les procédures)
  - Le nombre de paramètres
  - Pour chaque paramètre
    - Son numéro lexicographique
    - Un numéro correspondant à son type

Pour stocker les informations pour chaque nature, plusieurs méthodes sont possibles mais nous allons choisir ici d'utiliser des listes chaînées.

##### Intégration <a name="p2242"></a>
La table des représentations doit elle-aussi être initialisée dans la fonction main du fichier YACC.  
Comme la sauvegarde des champs, dimensions et paramètre passe par l'utilisation d'une liste chapinée, une fois l'analyse faîte, il nous faudra libérer la mémoire utilisée par le tableau.
```c
int main(void) {
  init_thash();
  init_tdec();
  init_trep();

  yyparse();

  trep_free();
  return 0;
}
```

Il se pose maintenant la question de l'insertion d'entrées dans la table des représentations.  
Comment insérer une novuelle fonction, procédure, structure ou encore un nouveau tableau avec notre fichier YACC ?

Voici un bout du fichier YACC permettant de vérifier la validité d'une fonction:
```
	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF liste_parametres RETOURNE type_simple corps
;


	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres:
| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param: un_param
| liste_param POINT_VIRGULE un_param
;

	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple
;
```

Nous serions tenté ici d'insérer un nouveau champ dans la table des représentations en faisant comme suit:
```
	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF liste_parametres RETOURNE type_simple corps    {
		index_fonction = trep_nouvelle_entree(NATURE_FONCTION);
	}
;
```

Nous pourrions ensuite ajouter les paramètre un à un en faisant de la sorte:
```
	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple {
		trep_ajoute_fonction_param(index_fonction, $1, 0);
	}
;
```

Cependant, cela ne fonctionnera pas, car l'ordre des actions YACC n'est pas descendant.  
En effet, ce sont d'abord les paramètres qui seront analysés, donc ici insérés, mais l'index de la fonction dans la table des représentations sera alors inconnu.

Pour créer une nouvelle entrée et insérer des paramètres, il faudra plutôt faire comme cela:
```
	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF liste_parametres RETOURNE type_simple corps    {
    /* Si l'index de la fonction est null, c'est qu'il n'y a aucun paramètre, il faudra tout de même insérer une nouvelle entrée dans la table. */
    if (est_null_index_fonction_trep()) {
			set_index_fonction_trep(trep_nouvelle_entree(NATURE_FONCTION));
		}
    /* On reinitialise l'index de la fonction */
		reinitialise_index_fonction_trep();
	}
;


	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres:
| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param: un_param
| liste_param POINT_VIRGULE un_param
;

	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple {
		if (est_null_index_fonction_trep()) {
			set_index_fonction_trep(trep_nouvelle_entree(NATURE_FONCTION));
		}
    /* On utilise pour l'instant la valeur 0 pour le champ exec */
		trep_ajoute_fonction_param(get_index_fonction_trep(), $1, 0);
	}
;
```

Tout se passe dans la règle du nouveau paramètre.  
Il faudra cependant bien penser à rajouter une entrée dans le cas où il n'y a aucun paramètre dans notre fonction.

#### La table des régions <a name="p225"></a>
##### Explication <a name="p2251"></a>
La table des régions sert  à représenter les informations des différentes régions du programme.  
Cette table comporte les champs suivants:
- La taille de la zone de données
- Le niveau d'imbrication (nis) (=0 pour le programme principal)
- Le pointeur vers l'arbre abstrait des instructions de cette région

##### Intégration <a name="p2252"></a>
La table des régions n'a pas besoin d'être initialisée.

### Création de l'arbre <a name="p23"></a>
Afin d'effectuer l'analyse sémantique et pour générer le texte intermédiaire, il est nécessaire de construire un arbre lors de l'execution du programme YACC.

La création de cet arbre va nous ammener à changer un grand nombre d'elements dans notre programme actuel.

#### Structure <a name="p231"></a>
La structure de l'arbre ne ressemblera pas à la structure d'un arbre binaire classique.
La structure sera légèrement différente afin de faciliter le travail sur les opérrations ternaires par exemple (if/then/else).

```c
/* la structure de l'arbre */
typedef struct noeud {
    int nature;
    int valeur;
    struct noeud *fils_gauche;
    struct noeud *frere_droit;

} noeud;

typedef struct noeud *arbre;
```

On remarque ici le remplacement du fils droit habituel par un **frère** droit.

#### Fonctions <a name="p232"></a>
Les fonctions de l'arbre vont être différentes des fonctions classiques étant donné la structure particulière de l'arbre.

On retrouvera par exemple ces deux fonctions inhabituelles :
```c
/* Concatene l'arbre fils et l'arbre pere */
arbre concat_pere_fils(arbre pere, arbre fils);

/* Concatene l'arbre fils et l'arbre frere */
arbre concat_pere_frere(arbre pere, arbre frere);
```
#### Intégration <a name="p233"></a>
L'intégration de l'arbre recquiert des modifications importantes des fichiers LEX et YACC.

L'abre va se construire au fur et à mesure des règles YACC.
Par exemple, l'instruction:
```c
fonction ma_fonction() {
  int variable = 2;
}
```
devra construire un arbre dans le style suivant:
```c
declaration_fonction
|
|------ declaration_variable
    |
    |------ variable
    |
    |------ 2
```

Cela implique de retourner des valeurs de type "arbre" au sen même des règles YACC, ce qui n'est pas possible classiquement. Il est donc nécessaire de modifier le type de retour des règles YACC.

Pour cela, il faut rajouter au dessus de la déclaration des tokens la règle suivante:
```c
%union {
	arbre t_arbre;
	int t_entier;
}
```
Cette règle permet d'autoriser deux valeurs de retour pour les règles YACC. Ici ce sera soit un type arbre, soit un type entier.

Puis définir pour chaque règle YACC son type de retour.

```c
%type <t_arbre> affectation expression
%type <t_entier> variable
```

Ces deux règles permettent d'indiquer au programme YACC que la lecture d'une affectation retourner un objet de type "t_arbre" alors que la lecture d'une variable retournera un objet de type "t_entier".
  
Cela permettra ensuite d'écrire la règle suivante:
```c
affectation: variable OPAFF expression {
		$$ = concat_pere_fils(
			creer_noeud(A_AFFECT, -1), 
			concat_pere_frere(
				creer_noeud(A_IDF, $1),
				$3
			)
		);
	}
;
```

Il est important de noter que les variables A_AFFECT et A_IDF sont ici des constantes définies préalablement et permettant ensuite, lors de la lecture de l'arbre, de retrouver quel type d'opération est stockée dans le noeud.

## Conception de la machine virtuelle <a name="p3"></a>