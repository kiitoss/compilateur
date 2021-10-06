# Réalisation d'un compilateur et d'une machine virtuelle pour le langage CPYRR

Ce projet a pour but la création d'un compilateur et d'une machine virtuelle pour le langage CPYRR, un langage procédural à structure de blocs.

## Conception des programmes LEX et YACC
### Le programme LEX
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


### Le programme YACC
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


## Création du compilateur (analyses lexicale, synthaxique, sémantique et génération du texte intermédiaire)

## Conception de la machine virtuelle