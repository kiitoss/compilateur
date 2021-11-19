%{
  #include "src/inc/tlex.h"
  #include "src/inc/tdec.h"
  #include "src/inc/trep.h"
  #include "src/inc/treg.h"

  #include <stdio.h>
  #include <stdlib.h>

  int yylex(void);
  void yyerror(char *);

  int line;
%}

// Déclaration des tokens
%token PROG
%token DEBUT FIN
%token POINT_VIRGULE DEUX_POINTS VIRGULE POINT_POINT POINT
%token VARIABLE IDF
%token FONCTION RETOURNE
%token PROCEDURE
%token PARENTHESE_OUVRANTE PARENTHESE_FERMANTE CROCHET_OUVRANT CROCHET_FERMANT
%token ENTIER REEL BOOLEEN CARACTERE TYPE
%token PLUS MOINS MULT DIV OPAFF
%token TRUE FALSE
%token STRUCT FSTRUCT TABLEAU DE  
%token ET OU NON
%token SI ALORS SINON
%token TANT_QUE FAIRE 
%token CSTE_FORMAT LIRE ECRIRE
%token DOUBLE_EGAL

// Tokens provisoires
%token AFFICHE


%%



	/* Le programme doit commencer par le token PROG */
programme: PROG corps
      	 ;


	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps: liste_declarations liste_instructions
				| liste_instructions
        ;


	/* Une liste de déclarations comprend une ou plusieurs déclarations séparées par le token POINT_VIRGULE */
liste_declarations:	declaration POINT_VIRGULE
					| liste_declarations declaration POINT_VIRGULE
					;


	/* Une liste d'instructions commence par le token DEBUT et se termine par le token FIN */
liste_instructions: DEBUT suite_liste_inst FIN
					;


	/* La liste d'instructions est composée d'une ou plusieurs instructions terminé par le token POINT_VIRGULE */
suite_liste_inst: instruction POINT_VIRGULE
					| suite_liste_inst instruction POINT_VIRGULE
					;


	/** A modifier */
/* Une déclaration peut être une déclaration de variable ou de fonction */
declaration: declaration_variable
				| declaration_fonction
				| declaration_procedure
				| declaration_type
				;

	/* declaration d'un type */
declaration_type: TYPE IDF DEUX_POINTS suite_declaration_type
				;
	/*	une suite de declaration type c une liste de champs ou un tableau a une dimention prédefini*/
suite_declaration_type:		STRUCT liste_champs FSTRUCT
							| TABLEAU dimension DE nom_type
							;

	/*	*/
dimension:	CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT
			;
	/*	*/
liste_dimensions:	une_dimension
					| liste_dimensions VIRGULE une_dimension
					;

	/*	*/
une_dimension:	expression POINT_POINT expression
				;

	/*	*/
liste_champs:	un_champ
				| liste_champs POINT_VIRGULE un_champ
				;

	/*	*/
un_champ:	IDF DEUX_POINTS nom_type
			;

	/* Le type peut être un type simple (entier, reel...) ou un identificateur (variable...) */
nom_type:	type_simple
			| IDF
			;

	/** A modifier */
type_simple:    ENTIER
				| REEL
				| BOOLEEN
				| CARACTERE
				;


	/* Grammaire de déclaration d'une variable */
declaration_variable: VARIABLE IDF DEUX_POINTS nom_type {
    tdec_insere($2, NATURE_VARIABLE, get_region());
  }
;


	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF liste_parametres RETOURNE type_simple corps {
  if (est_null_index_fonction_trep()) {
    treg_nouvelle_entree();
  }
  tdec_insere($2, NATURE_FONCTION, get_region());
  if (est_null_index_fonction_trep()) {
    set_index_fonction_trep(trep_nouvelle_entree(NATURE_FONCTION));
  }
  reinitialise_index_fonction_trep();
  depile_region();
 }
;

/* Grammaire de déclaration d'une structure */
declaration_procedure: PROCEDURE IDF liste_parametres corps {
    if (est_null_index_fonction_trep()) {
       treg_nouvelle_entree();
     }
     tdec_insere($2, NATURE_PROCEDURE, get_region());
     if (est_null_index_fonction_trep()) {
       set_index_fonction_trep(trep_nouvelle_entree(NATURE_PROCEDURE));
     }
     reinitialise_index_fonction_trep();
     depile_region();
    }
						;

	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres: PARENTHESE_OUVRANTE PARENTHESE_FERMANTE
				| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
				;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param:	un_param
				| liste_param POINT_VIRGULE un_param
				;

	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple {
    if (est_null_index_fonction_trep()) {
      treg_nouvelle_entree();
      set_index_fonction_trep(trep_nouvelle_entree(NATURE_FONCTION));
    }
    trep_ajoute_fonction_param(get_index_fonction_trep(), $1, 0);
  }
;

	/** A modifier */
liste_variable:	expression									{ printf("%d\n", $1); }
              	| liste_variable POINT_VIRGULE expression	{ printf("%d\n", $3); }
              	;


	/** A modifier */
instruction:	AFFICHE expression	{ printf("%d\n", $2); }
				| condition
				| tant_que
				| affectation
				| RETOURNE
				| lire
				| ecrire
				;
lire: LIRE PARENTHESE_OUVRANTE liste_variable PARENTHESE_FERMANTE
	;

ecrire: ECRIRE PARENTHESE_OUVRANTE format suite_ecriture PARENTHESE_FERMANTE
	;

format: CSTE_FORMAT
		;

suite_ecriture:
			  |VIRGULE variable suite_ecriture
			;
	/** A modifier */
condition:	SI expression_booleenne
			ALORS liste_instructions
			SINON liste_instructions
			;

tant_que: TANT_QUE expression_booleenne FAIRE liste_instructions
			;

affectation: variable OPAFF expression
			;
variable: IDF
		| tableau
		| IDF POINT variable
		;
tableau: IDF CROCHET_OUVRANT expression CROCHET_FERMANT
		;



	/** A modifier */
expression: ENTIER { $$ = $1; }
| e1;

e1:  e1 PLUS e2              { $$ = $1 + $3; }                      
	| e1 MOINS e2        { $$ = $1 - $3; }                       
	| e2
	;
e2: e2 DIV e3		     { $$ = $1 / $3; }     
	| e2 MULT e3         { $$ = $1 * $3; }                           
	| e3
	;

e3: PARENTHESE_OUVRANTE liste_variable PARENTHESE_FERMANTE 
    | e4
	;
e4: variable
	| CSTE_FORMAT
	| ENTIER
	;

	/** A modifier */ 
expression_booleenne: ep1
| PARENTHESE_OUVRANTE ep1 PARENTHESE_FERMANTE 
;
ep1: ep1 ET ep2	{ $$ = $1 && $3; }
	|ep2
	;
ep2: ep2 OU ep3	{ $$ = $1 || $3; }
	|ep3
	;
ep3: NON ep4	{ $$ = !$2; }
	|ep4
	;
ep4:variable
	|CSTE_FORMAT
	;

%%



void yyerror(char *s) {
  fprintf(stderr, "ligne %d: %s\n", line, s);
  exit(-1);
}

int main(void) {
  init_thash();
  init_tdec();
  init_trep();
  init_pile_regions();

  yyparse();

  printf("\n\nAffichage de la table de hash-code:\n");
  thash_affiche();
  printf("\n\nAffichage de la table lexicographique:\n");
  tlex_affiche();
  printf("\n\nAffichage de la table des déclarations:\n");
  tdec_affiche();
  printf("\n\nAffichage de la table des représentations:\n");
  trep_affiche();
  printf("\n\nAffichage de la table des régions:\n");
  treg_affiche();
  printf("\n");

  trep_free();
  return 0;
}
