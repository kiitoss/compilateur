%{
	#include "src/inc/tlex.h"
	#include "src/inc/tdec.h"
	#include "src/inc/trep.h"
	#include "src/inc/treg.h"
	#include "src/inc/arbre.h"
	
	#include <stdio.h>
	#include <stdlib.h>

	#define AFFICHER_TABLES 1
	#define AFFICHER_ARBRE 0
	
	int yylex(void);
	void yyerror(char *);
	
	int line;

	void nouvelle_declaration(int index_tlex, int nature);
	void fin_nouvelle_declaration();

	arbre mon_arbre;
%}

%union {
	arbre t_arbre;
	int t_entier;
}


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
%token TRUE FALSE
%token STRUCT FSTRUCT TABLEAU DE  
%token ET OU NON
%token SI ALORS SINON
%token TANT_QUE FAIRE 
%token LIRE ECRIRE
%token DOUBLE_EGAL
%token OPAFF

%token <t_entier> IDF ENTIER BOOLEEN REEL CARACTERE CSTE_FORMAT

// Tokens provisoires
%token AFFICHE


%type <t_arbre> corps liste_declarations liste_instructions suite_liste_inst declaration
%type <t_arbre> declaration_variable declaration_fonction declaration_procedure declaration_type
%type <t_arbre> suite_declaration_type liste_champs nom_type dimension liste_dimensions une_dimension
%type <t_arbre> expression un_champ liste_parametres liste_param un_param
%type <t_arbre> liste_variable instruction condition tant_que affectation lire ecrire suite_ecriture
%type <t_arbre> expression_booleenne tableau variable 

%type <t_entier> type_simple format e1 e2 e3 e4 ep1 ep2 ep3 ep4


%%



	/* Le programme doit commencer par le token PROG */
programme: 
	| PROG corps {
		if (AFFICHER_ARBRE) {
			printf("Affichage d'un corps de programme :\n");
			afficher_arbre($2);
			printf("\n");
		} else {
			printf("Affichage de l'arbre désactivé.\n");
		}
	}
;


	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps: liste_declarations liste_instructions {
		$$ = concat_pere_fils(
			creer_noeud(A_CORPS, VALEUR_NULL), 
			concat_pere_frere(
				concat_pere_fils(creer_noeud(A_LISTE_DECL, VALEUR_NULL), $1),
				concat_pere_fils(creer_noeud(A_LISTE_INSTR, VALEUR_NULL), $2)
			)
		);
	}
	| liste_instructions {
		$$ = concat_pere_fils(
			creer_noeud(A_CORPS, VALEUR_NULL),
			concat_pere_fils(creer_noeud(A_LISTE_INSTR, VALEUR_NULL), $1)
		);
	}
;


	/* Une liste de déclarations comprend une ou plusieurs déclarations séparées par le token POINT_VIRGULE */
liste_declarations: declaration POINT_VIRGULE {
		$$ = $1;
	}
	| liste_declarations declaration POINT_VIRGULE {
		$$ = concat_pere_frere($1, $2);
	}
;


	/* Une liste d'instructions commence par le token DEBUT et se termine par le token FIN */
liste_instructions: {
		$$ = creer_arbre_vide();
	}
	| DEBUT suite_liste_inst FIN {
		$$ = $2;
	}
;


	/* La liste d'instructions est composée d'une ou plusieurs instructions terminé par le token POINT_VIRGULE */
suite_liste_inst: {
		$$ = creer_arbre_vide();
	}
	| instruction POINT_VIRGULE {
		$$ = $1;
	}
	| suite_liste_inst instruction POINT_VIRGULE {
		$$ = concat_pere_frere($1, $2);
	}
;


	/* Une déclaration peut être une déclaration de variable, de fonction, de procedure ou de type */
declaration: declaration_variable {
		$$ = $1;
	}
	| declaration_fonction {
		$$ = $1;
	}
	| declaration_procedure {
		$$ = $1;
	}
	| declaration_type {
		$$ = $1;
	}
;

	/* La déclaration d'un type commence par le token TYPE */
declaration_type: TYPE IDF DEUX_POINTS suite_declaration_type {
		tdec_nouvelle_entree($2, trep_nouvelle_entree(NATURE_STRUCTURE), NATURE_STRUCTURE, get_region());
		$$ = concat_pere_fils(
			creer_noeud(A_DECL_TYPE, VALEUR_NULL), 
			concat_pere_frere(
				creer_noeud(A_IDF, $2),
				$4
			)
		);
	}
;
	/* La suite de déclaration d'un type est soit une structure, soit un tableau */
suite_declaration_type: STRUCT liste_champs FSTRUCT {
		$$ = concat_pere_fils(
			creer_noeud(A_STRUCT, VALEUR_NULL),
			concat_pere_fils(
				creer_noeud(A_LISTE_CHAMPS, VALEUR_NULL),
				$2
			)
		);
	}
	| TABLEAU dimension DE nom_type {
		$$ = concat_pere_fils(
			creer_noeud(A_TAB, VALEUR_NULL),
			concat_pere_frere($2, $4)
		);
	}
;

	/* Les dimensions d'un tableau sont représentées entre crochet */
dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT {
		$$ = concat_pere_fils(creer_noeud(A_LISTE_DIM, VALEUR_NULL), $2);
	}
;

liste_dimensions:	une_dimension {
		$$ = $1;
	}
	| liste_dimensions VIRGULE une_dimension {
		$$ = concat_pere_frere($1, $3);
	}
;

une_dimension: expression POINT_POINT expression {
		$$ = concat_pere_fils(
			creer_noeud(A_DIM, VALEUR_NULL),
			concat_pere_frere($1, $3)
		);
	}
;


liste_champs: un_champ {
		$$ = $1;
	}
	| liste_champs POINT_VIRGULE un_champ {
		$$ = concat_pere_frere($1, $3);
	}
;

un_champ: IDF DEUX_POINTS nom_type {
		tdec_nouvelle_entree($1, get_global_index_type_tdec(), NATURE_VARIABLE, get_region());
		$$ = concat_pere_fils(
			creer_noeud(A_CHAMP, VALEUR_NULL),
			concat_pere_frere(creer_noeud(A_IDF, $1), $3)
		);
	}
;

	/* Le type peut être un type simple (entier, reel...) ou un identificateur (variable...) */
nom_type: type_simple {
		$$ = creer_noeud(A_TYPE, $1);
		set_global_index_type_tdec($1);
	}
	| IDF {
		$$ = creer_noeud(A_IDF, yylval.t_entier);
		/*
			TODO
			index_tdec = get_index_tdec(yylval.t_entier) ?
			--> var a:b ==> type a = type b ?
		*/
		set_global_index_type_tdec(yylval.t_entier);
	}
;

	/** A modifier */
type_simple: ENTIER {
		$$ = 0;
	}
	| REEL {
		$$ = 1;
	}
	| BOOLEEN {
		$$ = 2;
	}
	| CARACTERE {
		$$ = 3;
	}
;


	/* Grammaire de déclaration d'une variable */
declaration_variable: VARIABLE IDF DEUX_POINTS nom_type {
		tdec_nouvelle_entree($2, get_global_index_type_tdec(), NATURE_VARIABLE, get_region());
		$$ = concat_pere_fils(
			creer_noeud(A_DECL_VAR, VALEUR_NULL),
			concat_pere_frere(creer_noeud(A_IDF, $2), $4)
		);
  }
;


	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF {
		nouvelle_declaration(yylval.t_entier, NATURE_FONCTION);
	} liste_parametres RETOURNE type_simple corps {
		fin_nouvelle_declaration();

		$$ = concat_pere_fils(
			creer_noeud(A_DECL_FONC, VALEUR_NULL),
			concat_pere_frere(
				$4,
				concat_pere_frere(
					creer_noeud(A_TYPE, $6),
					$7
				)
			)
		);
	}
;
/* Grammaire de déclaration d'une structure */
declaration_procedure: PROCEDURE IDF {
		nouvelle_declaration(yylval.t_entier, NATURE_PROCEDURE);
	} liste_parametres corps {
		fin_nouvelle_declaration();

		$$ = concat_pere_fils(
			creer_noeud(A_DECL_PROC, VALEUR_NULL),
			concat_pere_frere(
				$4,
				$5
			)
		);
	}
;

	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres: PARENTHESE_OUVRANTE PARENTHESE_FERMANTE {
		$$ = creer_noeud(A_LISTE_PARAM, VALEUR_NULL);
	}
	| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE {
		$$ = concat_pere_fils(creer_noeud(A_LISTE_PARAM, VALEUR_NULL), $2);
	}
;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param: un_param {
		$$ = $1;
	}
	| liste_param POINT_VIRGULE un_param {
		$$ = concat_pere_frere($1, $3);
	}
;

	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple {
		trep_ajoute_fonction_param(get_global_index_trep(), $1, $3);
		$$ = concat_pere_fils(
			creer_noeud(A_PARAM, VALEUR_NULL),
			concat_pere_frere(creer_noeud(A_IDF, $1), creer_noeud(A_TYPE, $3))
		);
  }
;

	/** A modifier */
liste_variable: expression {
		$$ = $1;
	}
	| liste_variable POINT_VIRGULE expression {
		$$ = concat_pere_frere($1, $3);
	}
;

	/** A modifier */
instruction: AFFICHE expression {
		$$ = concat_pere_fils(
			creer_noeud(A_AFFICHE, VALEUR_NULL),
			$2
		);
	}
	| condition {
		$$ = $1;
	}
	| tant_que {
		$$ = $1;
	}
	| affectation {
		$$ = $1;
	}
	| RETOURNE {
		$$ = creer_noeud(A_RETOURNE, VALEUR_NULL);
	}
	| lire {
		$$ = $1;
	}
	| ecrire {
		$$ = $1;
	}
;
lire: LIRE PARENTHESE_OUVRANTE liste_variable PARENTHESE_FERMANTE {
		$$ = concat_pere_fils(
			creer_noeud(A_LIRE, VALEUR_NULL),
			$3
		);
	}
;
ecrire: ECRIRE PARENTHESE_OUVRANTE format suite_ecriture PARENTHESE_FERMANTE {
		$$ = concat_pere_fils(
			creer_noeud(A_ECRIRE, VALEUR_NULL),
			concat_pere_frere(creer_noeud(A_FORMAT, $3), $4)
		);
	}
;

	/* format retourne provisoirement 0 */
format: CSTE_FORMAT {
		$$ = 0;
	}
;

	/* suite_ecriture retourne provisoirement un arbre vide */
suite_ecriture: {
		$$ = creer_noeud(VALEUR_NULL, VALEUR_NULL);
	}
	| VIRGULE variable suite_ecriture {
		$$ = creer_noeud(VALEUR_NULL, VALEUR_NULL);
	}
;

	/** A modifier */
condition: SI expression_booleenne
	ALORS liste_instructions
	SINON liste_instructions {
		$$ = concat_pere_fils(
			creer_noeud(A_CONDITION, VALEUR_NULL),
			concat_pere_frere(
				concat_pere_fils(
					creer_noeud(A_SI, VALEUR_NULL),
					$2
				),
				concat_pere_frere(
					concat_pere_fils(
						creer_noeud(A_ALORS, VALEUR_NULL),
						$4
					),
					concat_pere_fils(
						creer_noeud(A_SINON, VALEUR_NULL),
						$6
					)
				)
			)
		);
	}
;
tant_que: TANT_QUE expression_booleenne FAIRE liste_instructions {
		$$ = concat_pere_fils(
			creer_noeud(A_TANT_QUE, VALEUR_NULL),
			concat_pere_fils($2, $4)
		);
	}
;
affectation: variable OPAFF expression {
		$$ = concat_pere_fils(
			creer_noeud(A_AFFECT, VALEUR_NULL), 
			concat_pere_frere(
				$1,
				$3
			)
		);
	}
;
variable: IDF {
		$$ = creer_noeud(A_IDF, $1);
	}
	| tableau {
		$$ = $1;
	}
	| IDF POINT variable {
		$$ = creer_noeud(A_IDF, $1);
	}
;
tableau: IDF CROCHET_OUVRANT expression CROCHET_FERMANT {
		$$ = concat_pere_fils(
			creer_noeud(A_IDF, $1),
			$3
		);
	}
;



	/** A modifier */
expression: ENTIER {
		$$ = creer_noeud(A_IDF, $1);
	}
	| e1 {
		$$ = creer_noeud(A_IDF, $1);
	}
;
e1:  e1 PLUS e2 {
		$$ = $1 + $3;
	}                      
	| e1 MOINS e2 {
		$$ = $1 - $3;
	}                       
	| e2 {
		$$ = VALEUR_NULL;
	}
;
e2: e2 DIV e3 {
		$$ = $1 / $3;
	}     
	| e2 MULT e3 {
		$$ = $1 * $3;
	}                           
	| e3 {
		$$ = $1;
	}
;
e3: PARENTHESE_OUVRANTE liste_variable PARENTHESE_FERMANTE {
		$$ = VALEUR_NULL;
	}
	| e4 {
		$$ = VALEUR_NULL;
	}
;
e4: variable {
		$$ = VALEUR_NULL;
	}
	| CSTE_FORMAT {
		$$ = VALEUR_NULL;
	}
	| ENTIER {
		$$ = VALEUR_NULL;
	}
;

	/** A modifier */ 
expression_booleenne: ep1 {
		$$ = creer_noeud(A_IDF, $1);
	}
	| PARENTHESE_OUVRANTE ep1 PARENTHESE_FERMANTE {
		$$ = creer_noeud(A_IDF, $2);
	}
;
ep1: ep1 ET ep2 {
		$$ = $1 && $3;
	}
	| ep2 {
		$$ = VALEUR_NULL;
	}
;
ep2: ep2 OU ep3 {
		$$ = $1 || $3;
	}
	| ep3 {
		$$ = VALEUR_NULL;
	}
;
ep3: NON ep4 {
		$$ = !$2;
	}
 | ep4 {
		$$ = VALEUR_NULL;
	}
;
ep4: variable {
		$$ = 1;
	}
	| CSTE_FORMAT {
		$$ = 1;
	}
;
%%


void nouvelle_declaration(int index_tlex, int nature) {
	treg_nouvelle_entree();
	set_index_global_trep(trep_nouvelle_entree(nature));
	tdec_nouvelle_entree(index_tlex, get_global_index_trep(), nature, get_region());
}

void fin_nouvelle_declaration() {
	reinitialise_index_global_trep();
	depile_region();
}

/* Insere les types de base dans les tables */
void insere_types_base() {
    int index_tlex_entier, index_tlex_reel, index_tlex_booleen, index_tlex_caractere;
    index_tlex_entier = tlex_insere("entier");
    index_tlex_reel = tlex_insere("reel");
    index_tlex_booleen = tlex_insere("booleen");
    index_tlex_caractere = tlex_insere("caractere");
    tdec_nouvelle_entree(index_tlex_entier, 0, NATURE_VARIABLE, 0);
    tdec_nouvelle_entree(index_tlex_reel, 1, NATURE_VARIABLE, 0);
    tdec_nouvelle_entree(index_tlex_booleen, 2, NATURE_VARIABLE, 0);
    tdec_nouvelle_entree(index_tlex_caractere, 3, NATURE_VARIABLE, 0);
}

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(-1);
}


int main(void) {
	init_thash();
	init_tdec();
	init_trep();
	init_pile_regions();

	mon_arbre = creer_arbre_vide();

	insere_types_base();
	yyparse();

	if (AFFICHER_TABLES) {
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
	} else {
		printf("Affichage des tables désactivé.\n");
	}

	trep_free();
  return 0;
}
