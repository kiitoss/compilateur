%{
	#include "inc/utils.h"
    #include "inc/arbre.h"

	#define AFFICHER_TABLES 1
	#define AFFICHER_ARBRE 1
	
	int yylex(void);
	void yyerror(char *);
	
	int line;

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
%token DOUBLE_EGAL SUPERIEUR INFERIEUR SUP_EGAL INF_EGAL
%token OPAFF
%token TYPE_ENTIER TYPE_REEL TYPE_BOOLEEN TYPE_CARACTERE VRAI FAUX

%token <t_entier> IDF ENTIER BOOLEEN REEL CARACTERE CSTE_FORMAT

// Tokens provisoires
%token AFFICHE


%type <t_arbre> corps liste_declarations liste_instructions suite_liste_inst declaration
%type <t_arbre> declaration_variable declaration_fonction declaration_procedure declaration_type
%type <t_arbre> suite_declaration_type liste_champs nom_type dimension liste_dimensions une_dimension
%type <t_arbre> expression un_champ liste_parametres liste_param un_param liste_arguments liste_args un_arg
%type <t_arbre> instruction condition tant_que affectation
%type <t_arbre> expression_booleenne tableau variable appel type_simple

%type <t_entier> booleen expression_arithmetiques exp 


%%



	/* Le programme doit commencer par le token PROG */
programme: 
	| PROG corps {
		if (AFFICHER_ARBRE) {
			printf("Affichage d'un corps de programme :\n");
			arbre_affiche($2);
			printf("\n");
		} else {
			printf("Affichage de l'arbre désactivé.\n");
		}
	}
;


	/* Le corps du programme est une liste de déclarations puis d'instructions */
corps: liste_declarations liste_instructions {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_CORPS, VALEUR_NULL), 
			arbre_concat_pere_frere(
				arbre_concat_pere_fils(arbre_creer_noeud(A_LISTE_DECL, VALEUR_NULL), $1),
				arbre_concat_pere_fils(arbre_creer_noeud(A_LISTE_INSTR, VALEUR_NULL), $2)
			)
		);
    }
    | liste_instructions {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_CORPS, VALEUR_NULL),
			arbre_concat_pere_fils(arbre_creer_noeud(A_LISTE_INSTR, VALEUR_NULL), $1)
		);
	}
;


	/* Une liste de déclarations comprend une ou plusieurs déclarations séparées par le token POINT_VIRGULE */
liste_declarations: declaration POINT_VIRGULE {
		$$ = $1;
	}
	| liste_declarations declaration POINT_VIRGULE {
		$$ = arbre_concat_pere_frere($1, $2);
	}
;


	/* Une liste d'instructions commence par le token DEBUT et se termine par le token FIN */
liste_instructions: {
		$$ = arbre_creer_arbre_vide();
	}
	| DEBUT suite_liste_inst FIN {
		$$ = $2;
	}
;


	/* La liste d'instructions est composée d'une ou plusieurs instructions terminé par le token POINT_VIRGULE */
suite_liste_inst: {
		$$ = arbre_creer_arbre_vide();
	}
	| instruction POINT_VIRGULE {
		$$ = $1;
	}
	| suite_liste_inst instruction POINT_VIRGULE {
		$$ = arbre_concat_pere_frere($1, $2);
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
declaration_type: TYPE IDF { maj_tlex_index($2); } DEUX_POINTS suite_declaration_type {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_DECL_TYPE, VALEUR_NULL), 
			arbre_concat_pere_frere(
				arbre_creer_noeud(A_IDF, $2),
				$5
			)
		);
    }
;
	/* La suite de déclaration d'un type est soit une structure, soit un tableau */
suite_declaration_type: STRUCT { debut_nouvelle_structure(); } liste_champs FSTRUCT {
        fin_nouvelle_structure();
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_STRUCT, VALEUR_NULL),
			arbre_concat_pere_fils(
				arbre_creer_noeud(A_LISTE_CHAMPS, VALEUR_NULL),
				$3
			)
		);
    }
	| TABLEAU { debut_nouveau_tableau(); } dimension DE nom_type {
        fin_nouveau_tableau($5->valeur);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_TAB, VALEUR_NULL),
			arbre_concat_pere_frere($3, $5)
		);
    }
;

	/* Les dimensions d'un tableau sont représentées entre crochet */
dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT {
		$$ = arbre_concat_pere_fils(arbre_creer_noeud(A_LISTE_DIM, VALEUR_NULL), $2);
	}
;

liste_dimensions: une_dimension {
		$$ = $1;
	}
	| liste_dimensions VIRGULE une_dimension {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

une_dimension: expression POINT_POINT expression {
        nouvelle_dimension($1->valeur, $3->valeur);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_DIM, VALEUR_NULL),
			arbre_concat_pere_frere($1, $3)
		);
    }
;

liste_champs: un_champ {
		$$ = $1;
	}
    | liste_champs POINT_VIRGULE un_champ {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

un_champ: IDF DEUX_POINTS nom_type {
        nouveau_champ($3->valeur);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_CHAMP, VALEUR_NULL),
			arbre_concat_pere_frere(arbre_creer_noeud(A_IDF, $1), $3)
		);
    };

	/* Le type peut être un type simple (entier, reel...) ou un identificateur (variable...) */
nom_type: type_simple {
        $$ = arbre_creer_noeud(A_TYPE, $1->valeur);
	}
	| IDF {
        $$ = arbre_creer_noeud(A_IDF, yylval.t_entier);
    }
;

type_simple: TYPE_ENTIER {
        $$ = arbre_creer_noeud(A_TYPE_B, 0); 
    }
	| TYPE_REEL {
        $$ = arbre_creer_noeud(A_TYPE_B, 1); 
    }
	| TYPE_BOOLEEN {
        $$ = arbre_creer_noeud(A_TYPE_B, 2); 
    }
	| TYPE_CARACTERE {
        $$ = arbre_creer_noeud(A_TYPE_B, 3); 
    }
;


	/* Grammaire de déclaration d'une variable */
declaration_variable: VARIABLE IDF DEUX_POINTS nom_type {
        nouvelle_variable($2, $4->valeur);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_DECL_VAR, VALEUR_NULL),
			arbre_concat_pere_frere(arbre_creer_noeud(A_IDF, $2), $4)
		);
    }
;


	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF { debut_nouvelle_fonction_ou_procedure(FONC); } liste_parametres RETOURNE type_simple corps {
        fin_nouvelle_fonction_ou_procedure(FONC, $6->valeur);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_DECL_FONC, VALEUR_NULL),
			arbre_concat_pere_frere(
				$4,
				arbre_concat_pere_frere(
					$6,
					$7
				)
			)
		);
	}
;
/* Grammaire de déclaration d'une procedure */
declaration_procedure: PROCEDURE IDF { debut_nouvelle_fonction_ou_procedure(PROC); } liste_parametres corps {
	    fin_nouvelle_fonction_ou_procedure(PROC, 0);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_DECL_PROC, VALEUR_NULL),
			arbre_concat_pere_frere(
				$4,
				$5
			)
		);
    }
;

	/* La liste des paramètres est soit vide soit entourée de parenthèses */
liste_parametres: PARENTHESE_OUVRANTE PARENTHESE_FERMANTE {
        $$ = arbre_creer_noeud(A_LISTE_PARAM, VALEUR_NULL);
    }
	| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE {
		$$ = arbre_concat_pere_fils(arbre_creer_noeud(A_LISTE_PARAM, VALEUR_NULL), $2);
	}
;


	/* La liste des paramètres comprend un ou plus paramètres séparés par le token POINT_VIRGULE */
liste_param: un_param {
		$$ = $1;
	}
	| liste_param POINT_VIRGULE un_param {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

	/* Grammaire d'un paramètre */
un_param: IDF DEUX_POINTS type_simple {
        nouveau_parametre($1, $3->valeur);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_PARAM, VALEUR_NULL),
			arbre_concat_pere_frere(arbre_creer_noeud(A_IDF, $1), arbre_creer_noeud(A_TYPE, $3->valeur))
		);
    }
;

instruction: condition { $$ = $1; }
	| tant_que { $$ = $1; }
	| affectation { $$ = $1; }
	| appel { $$ = $1; }
	| test_arithmetiques { $$ = arbre_creer_noeud(VALEUR_NULL, 1); }
	| RETOURNE {
		$$ = arbre_creer_noeud(A_RETOURNE, VALEUR_NULL);
	}
;
	/*  */
test_arithmetiques: PARENTHESE_OUVRANTE expression DOUBLE_EGAL expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression SUPERIEUR expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression INFERIEUR expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression SUP_EGAL expression PARENTHESE_FERMANTE
	| PARENTHESE_OUVRANTE expression INF_EGAL expression PARENTHESE_FERMANTE
;

liste_arguments: {
		$$ = arbre_creer_noeud(A_LISTE_ARGS, VALEUR_NULL);
	}
	| PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE {
		$$ = arbre_concat_pere_fils(arbre_creer_noeud(A_LISTE_ARGS, VALEUR_NULL), $2);
	}
;


	/* La liste des arguments comprend un ou plus arguments séparés par le token VIRGULE */
liste_args: un_arg {
		$$ = $1;
	}
	| liste_args VIRGULE un_arg {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

	/* Grammaire d'un argument */
un_arg: expression {
		$$ = arbre_creer_arbre_vide();
  }
;

condition: SI expression_booleenne
	ALORS liste_instructions
	SINON liste_instructions {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_CONDITION, VALEUR_NULL),
			arbre_concat_pere_frere(
				arbre_concat_pere_fils(
					arbre_creer_noeud(A_SI, VALEUR_NULL),
					$2
				),
				arbre_concat_pere_frere(
					arbre_concat_pere_fils(
						arbre_creer_noeud(A_ALORS, VALEUR_NULL),
						$4
					),
					arbre_concat_pere_fils(
						arbre_creer_noeud(A_SINON, VALEUR_NULL),
						$6
					)
				)
			)
		);
	}
;

tant_que: TANT_QUE expression_booleenne FAIRE liste_instructions {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_TANT_QUE, VALEUR_NULL),
			arbre_concat_pere_fils($2, $4)
		);
	}
;

affectation: variable OPAFF expression {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_AFFECT, VALEUR_NULL), 
			arbre_concat_pere_frere(
				$1,
				$3
			)
		);
	}
;

variable: IDF {
		$$ = arbre_creer_noeud(A_IDF, $1);
	}
	| tableau {
		$$ = $1;
	}
	| variable POINT variable {

	}
	| IDF POINT variable {
		$$ = arbre_creer_noeud(A_IDF, $1);
	}
;

tableau: IDF CROCHET_OUVRANT expression CROCHET_FERMANT {
    $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_IDF, $1),
			$3
		);
    }
;

appel: IDF liste_arguments {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_IDF, $1),
			$2
		);
    }
;

expression: expression_arithmetiques {
		$$ = arbre_creer_noeud(A_IDF, $1);
	}
	| expression_booleenne {
		$$ = $1;
	}
;

expression_arithmetiques: expression_arithmetiques PLUS exp {
		$$ = $1 + $3; 
	}
	| expression_arithmetiques MOINS exp {
		$$ = $1 - $3;
	}
	| expression_arithmetiques MULT exp {
		$$ = $1 * $3;
	}
	| expression_arithmetiques DIV exp {
		$$ = $1 / $3;
	}
	| exp {
 		$$ = VALEUR_NULL;
	}
;

exp: variable {
		$$ = VALEUR_NULL;
	}
	| CSTE_FORMAT {
		$$ = VALEUR_NULL;
	}
	| appel {
		$$ = VALEUR_NULL;
	}
	| ENTIER {
		$$ = VALEUR_NULL;
	}
	| REEL {
		$$ = VALEUR_NULL;
	}
	| exp VIRGULE expression {
		$$ = VALEUR_NULL;
	}

	| liste_arguments {
		$$ = VALEUR_NULL;
	}
;

	/** A modifier */ 
expression_booleenne: booleen {
		$$ = arbre_creer_noeud(A_IDF, $1);
	}
	| booleen ET booleen {
		$$ = arbre_creer_noeud(A_IDF, ($1 && $3));
	}
	| booleen OU booleen {
		$$ = arbre_creer_noeud(A_IDF, ($1 || $3));
	}
	| PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE {
		$$ = $2;
	}
	| expression_booleenne ET expression_booleenne {
	}
	| expression_booleenne OU expression_booleenne {
	}
;

booleen: variable {
		$$ = $1->valeur;
	}
	| CSTE_FORMAT {
		$$ = $1;
	}
	| NON booleen {
		$$ = !$2;
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
		printf("\n\nAffichage de la table de hash-code:\n");
		thash_affiche();
		printf("\n\nAffichage de la table lexicographique:\n");
		tlex_affiche();
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
