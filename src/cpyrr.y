%{
	#include "inc/utils.h"

	#define AFFICHER_TABLES 1
	#define AFFICHER_ARBRE 0
	
	int yylex(void);
	void yyerror(char *);
	
	int line;
%}


// Déclaration des tokens
%token PROG
%token DEBUT FIN
%token POINT_VIRGULE DEUX_POINTS VIRGULE POINT_POINT POINT
%token VARIABLE
%token FONCTION RETOURNE
%token PROCEDURE
%token PARENTHESE_OUVRANTE PARENTHESE_FERMANTE CROCHET_OUVRANT CROCHET_FERMANT
%token TYPE
%token PLUS MOINS MULT DIV 
%token STRUCT FSTRUCT TABLEAU DE
%token VRAI FAUX AFFICHE
%token ET OU NON
%token SI ALORS SINON
%token TANT_QUE FAIRE 
%token DOUBLE_EGAL SUPERIEUR INFERIEUR SUP_EGAL INF_EGAL
%token OPAFF
%token IDF ENTIER BOOLEEN REEL CARACTERE CSTE_FORMAT
%token TYPE_ENTIER TYPE_REEL TYPE_BOOLEEN TYPE_CARACTERE
%%



	/* Le programme doit commencer par le token PROG */
programme: 
	| PROG corps {
		if (AFFICHER_ARBRE) {
		} else {
		}
	}
;


	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps: liste_declarations liste_instructions {
	}
	| liste_instructions {
	}
;


	/* Une liste de déclarations comprend une ou plusieurs déclarations séparées par le token POINT_VIRGULE */
liste_declarations: declaration POINT_VIRGULE {
	}
	| liste_declarations declaration POINT_VIRGULE {
	}
;


	/* Une liste d'instructions commence par le token DEBUT et se termine par le token FIN */
liste_instructions: {
	}
	| DEBUT suite_liste_inst FIN {
	}
;


	/* La liste d'instructions est composée d'une ou plusieurs instructions terminé par le token POINT_VIRGULE */
suite_liste_inst: {
	}
	| instruction POINT_VIRGULE {
	}
	| suite_liste_inst instruction POINT_VIRGULE {
	}
;


	/* Une déclaration peut être une déclaration de variable, de fonction, de procedure ou de type */
declaration: declaration_variable {
	}
	| declaration_fonction {
	}
	| declaration_procedure {
	}
	| declaration_type {
	}
;

	/* La déclaration d'un type commence par le token TYPE */
declaration_type: TYPE IDF { maj_tlex_index($2); } DEUX_POINTS suite_declaration_type {
        
	}
;
	/* La suite de déclaration d'un type est soit une structure, soit un tableau */
suite_declaration_type: STRUCT { debut_nouvelle_structure(); } liste_champs FSTRUCT { fin_nouvelle_structure(); }
	| TABLEAU { debut_nouveau_tableau(); } dimension DE nom_type { fin_nouveau_tableau($5); }
;

	/* Les dimensions d'un tableau sont représentées entre crochet */
dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT {
	}
;

liste_dimensions: une_dimension | liste_dimensions VIRGULE une_dimension
;

une_dimension: expression POINT_POINT expression { nouvelle_dimension($1, $3); }
;

liste_champs: un_champ | liste_champs POINT_VIRGULE un_champ;

un_champ: IDF DEUX_POINTS nom_type { nouveau_champ($3); };

	/* Le type peut être un type simple (entier, reel...) ou un identificateur (variable...) */
nom_type: type_simple { $$ = $1; }
	| IDF { $$ = $1; }
;

	/** A modifier */
type_simple: TYPE_ENTIER { $$ = 0; }
	| TYPE_REEL { $$ = 1; }
	| TYPE_BOOLEEN { $$ = 2; }
	| TYPE_CARACTERE { $$ = 3; }
;


	/* Grammaire de déclaration d'une variable */
declaration_variable: VARIABLE IDF DEUX_POINTS nom_type { nouvelle_variable($4); }
;


	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF {
	} liste_parametres RETOURNE type_simple corps {
	}
;
/* Grammaire de déclaration d'une procedure */
declaration_procedure: PROCEDURE IDF {
	} liste_parametres corps {
	}
;

	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres: PARENTHESE_OUVRANTE PARENTHESE_FERMANTE {
	}
	| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE {
	}
;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param: un_param {
	}
	| liste_param POINT_VIRGULE un_param {
	}
;

	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple {
    }
;

	/** A modifier */
instruction: condition {
	}
	| tant_que {
	}
	| affectation {
	}
	| appel {
	}
	| test_arithmetiques {
	}
	| RETOURNE {
	}
;
	/*  */
test_arithmetiques: PARENTHESE_OUVRANTE expression DOUBLE_EGAL expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression SUPERIEUR expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression INFERIEUR expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression SUP_EGAL expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression INF_EGAL expression PARENTHESE_FERMANTE
;

	/* A completer */
liste_arguments: {
	}
	| PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE {
	}
;


	/* La liste des arguments comprend un ou plus arguments séparés par le token VIRGULE */
liste_args: un_arg {
	}
	| liste_args VIRGULE un_arg {
	}
;

	/* Grammaire d'un argument */
un_arg: expression {
  }
;

	/** A modifier */
condition: SI expression_booleenne
	ALORS liste_instructions
	SINON liste_instructions {
	}
;
tant_que: TANT_QUE expression_booleenne FAIRE liste_instructions {
	}
;
affectation: variable OPAFF expression {
	}
;
variable: IDF {
	}
	| tableau {
	}
	| variable POINT variable {

	}
	| IDF POINT variable {
	}
;

tableau: IDF CROCHET_OUVRANT expression CROCHET_FERMANT {
	}
;

	/* A completer */
appel: IDF liste_arguments {
	}
;

	/** A modifier */
expression: expression_arithmetiques {
	}
	| expression_booleenne {
	}
;

expression_arithmetiques: expression_arithmetiques PLUS exp {
	}
	| expression_arithmetiques MOINS exp {
	}
	| expression_arithmetiques MULT exp {
	}
	| expression_arithmetiques DIV exp {
	}
	| exp {
	}
;

exp: variable {
	}
	| CSTE_FORMAT {
	}
	| appel {
	}
	| ENTIER {
        $$ = $1;
	}
	| REEL {
        $$ = $1;
	}
	| exp VIRGULE expression {
	}

	| liste_arguments {
	}
;

	/** A modifier */ 
expression_booleenne: booleen {
	}
	| booleen ET booleen {
	}
	| booleen OU booleen {
	}
	| PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE {
	}
	| expression_booleenne ET expression_booleenne {
	}
	| expression_booleenne OU expression_booleenne {
	}
;

booleen: variable {
	}
	| CSTE_FORMAT {
	}
	| NON booleen {
	}
	| test_arithmetiques {
	}
;

%%

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(-1);
}


int main(void) {
    init_tables();

	yyparse();

	if (AFFICHER_TABLES) {
		/* printf("\n\nAffichage de la table de hash-code:\n");
		thash_affiche();
		printf("\n\nAffichage de la table lexicographique:\n");
		tlex_affiche(); */
		printf("\n\nAffichage de la table des declarations:\n");
		tdec_affiche();
        printf("\n\nAffichage de la table des representations:\n");
		trep_affiche();
        printf("\n\nAffichage de la table des regions:\n");
		treg_affiche();
		printf("\n");
	} else {
		printf("Affichage des tables désactivé.\n");
	}

	return EXIT_SUCCESS;
}
