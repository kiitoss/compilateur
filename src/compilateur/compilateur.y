%{
    #include "inc/compilateur.h"
    #include "inc/arbre.h"
    #include <unistd.h>

    extern FILE *yyin;
    extern FILE *yyout;
	
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
%token PROG DEBUT FIN
%token POINT_VIRGULE DEUX_POINTS VIRGULE POINT_POINT POINT
%token TYPE_ENTIER TYPE_REEL TYPE_BOOLEEN TYPE_CARACTERE
%token FONCTION PROCEDURE PARENTHESE_OUVRANTE PARENTHESE_FERMANTE RETOURNE
%token TABLEAU DE CROCHET_OUVRANT CROCHET_FERMANT
%token STRUCT FSTRUCT 
%token TYPE VARIABLE IDF ENTIER REEL
%token SI ALORS SINON VRAI FAUX
%token TANT_QUE FAIRE 
%token PLUS MOINS MULT DIV 
%token DOUBLE_EGAL SUPERIEUR INFERIEUR SUP_EGAL INF_EGAL
%token ET OU NON
%token OPAFF
%token ECRIT GUILLEMENT CARACTERE


%type <t_arbre> corps liste_declarations liste_instructions suite_liste_inst
%type <t_arbre> declaration declaration_type suite_declaration_type declaration_variable declaration_fonction declaration_procedure
%type <t_arbre> liste_parametres liste_param un_param liste_arguments liste_args un_arg
%type <t_arbre> instruction condition tant_que affectation
%type <t_arbre> dimension liste_dimensions une_dimension lecture_dimensions
%type <t_arbre> liste_champs un_champ
%type <t_arbre> variable nom_type type_simple
%type <t_arbre> appel expression expression_arithmetiques expression_booleenne resultat_retourne booleen test_arithmetiques exp lecture_tableau
%type <t_arbre> ecrit

%type <t_entier> idf

%%
programme:
    | PROG corps {
        affecte_arbre_region_en_cours($2);
    }
;

idf: IDF {
        $$ = yylval.t_entier;
    }
;

corps: liste_declarations liste_instructions {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_CORPS), 
			arbre_concat_pere_frere(
				arbre_concat_pere_fils(arbre_creer_noeud_vide(A_LISTE_DECL), $1),
				arbre_concat_pere_fils(arbre_creer_noeud_vide(A_LISTE_INSTR), $2)
			)
		);
    }
    | liste_instructions {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_CORPS),
			arbre_concat_pere_fils(arbre_creer_noeud_vide(A_LISTE_INSTR), $1)
		);
    }
;

liste_declarations: declaration POINT_VIRGULE {
		$$ = $1;
	}
    | liste_declarations declaration POINT_VIRGULE {
		$$ = arbre_concat_pere_frere($1, $2);
	}
;

liste_instructions: {
		$$ = arbre_creer_arbre_vide();
	}
	| DEBUT suite_liste_inst FIN {
		$$ = $2;
	}
;

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

declaration_type: TYPE idf { maj_tlex_index($2); } DEUX_POINTS suite_declaration_type {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud(A_DECL_TYPE, $2, VALEUR_NULL), 
			$5
		);
    }
;

suite_declaration_type: STRUCT { debut_nouvelle_structure(); } liste_champs FSTRUCT {
        fin_nouvelle_structure();
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_STRUCT),
			$3
		);
    }
	| TABLEAU { debut_nouveau_tableau(); } dimension DE nom_type {
        fin_nouveau_tableau($5->valeur_1);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_TAB),
			arbre_concat_pere_frere($3, $5)
		);
    }
;

dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT {
		$$ = $2;
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
        int e1 = $1->valeur_1;
        int e2 = $3->valeur_1;

        nouvelle_dimension(e1, e2);
        $$ = arbre_creer_noeud(A_DIM, e1, e2);
    }
;

liste_champs: un_champ {
		$$ = $1;
	}
    | liste_champs POINT_VIRGULE un_champ {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

un_champ: idf DEUX_POINTS nom_type {
        int tlex_index_type = $3->valeur_1;
        nouveau_champ($1, tlex_index_type);
        $$ = arbre_creer_noeud(A_CHAMP, $1, tlex_index_type);
    }
;

nom_type: type_simple {
        int tlex_index_type = $1->valeur_1;
        $$ = arbre_creer_noeud(A_TYPE, tlex_index_type, VALEUR_NULL);
	}
	| IDF {
        $$ = arbre_creer_noeud(A_TYPE, yylval.t_entier, VALEUR_NULL);
    }
;

type_simple: TYPE_ENTIER {
        $$ = arbre_creer_noeud(A_TYPE, 0, VALEUR_NULL); 
    }
	| TYPE_REEL {
        $$ = arbre_creer_noeud(A_TYPE, 1, VALEUR_NULL); 
    }
	| TYPE_BOOLEEN {
        $$ = arbre_creer_noeud(A_TYPE, 2, VALEUR_NULL); 
    }
	| TYPE_CARACTERE {
        $$ = arbre_creer_noeud(A_TYPE, 3, VALEUR_NULL); 
    }
;

declaration_variable: VARIABLE idf DEUX_POINTS nom_type {
        int tlex_index_type = $4->valeur_1;
        nouvelle_variable($2, tlex_index_type);
        $$ = arbre_creer_noeud(A_DECL_VAR, $2, tlex_index_type);
    }
;

declaration_fonction: FONCTION idf { debut_nouvelle_fonction_ou_procedure(FONC, $2); } liste_parametres RETOURNE type_simple  {
        int tlex_index_type = $6->valeur_1;
        fin_nouvelle_fonction_ou_procedure(FONC, tlex_index_type);
	} corps {
        affecte_arbre_region_en_cours(arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_DECL_FONC),
			arbre_concat_pere_frere(
				$4,
				arbre_concat_pere_frere(
					$6,
					$8
				)
			)
		));

        quitte_nouvelle_fonction_ou_procedure();
        
        $$ = arbre_creer_noeud(A_DECL_FONC, $2, VALEUR_NULL);
    }
;

declaration_procedure: PROCEDURE idf { debut_nouvelle_fonction_ou_procedure(PROC, $2); } liste_parametres {
	    fin_nouvelle_fonction_ou_procedure(PROC, 0);
    } corps {
        affecte_arbre_region_en_cours(arbre_concat_pere_fils(
			arbre_creer_noeud(A_DECL_PROC, $2, VALEUR_NULL),
			arbre_concat_pere_frere(
				$4,
				$6
			)
		));

        quitte_nouvelle_fonction_ou_procedure();

        $$ = arbre_creer_noeud(A_DECL_PROC, $2, VALEUR_NULL);
    }
;

liste_parametres: PARENTHESE_OUVRANTE PARENTHESE_FERMANTE {
        $$ = arbre_creer_noeud_vide(A_LISTE_PARAM);
    }
	| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE {
		$$ = arbre_concat_pere_fils(arbre_creer_noeud_vide(A_LISTE_PARAM), $2);
	}
;

liste_param: un_param {
		$$ = $1;
	}
	| liste_param POINT_VIRGULE un_param {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

un_param: IDF DEUX_POINTS type_simple {
        int tlex_index_type = $3->valeur_1;
        nouveau_parametre(yylval.t_entier, tlex_index_type);
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_PARAM),
			arbre_concat_pere_frere(arbre_creer_noeud(A_IDF, yylval.t_entier, VALEUR_NULL), arbre_creer_noeud(A_TYPE, tlex_index_type, VALEUR_NULL))
		);
    }
;

instruction: condition {
        $$ = $1;
    }
	| tant_que {
        $$ = $1;
    }
	| affectation { 
        $$ = $1;
    }
	| appel {
        $$ = $1;
    }
    | ecrit {
        $$ =$1;
    }
	| RETOURNE resultat_retourne {
		$$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_RETOURNE),
            $2
        );
	}
;

ecrit: ECRIT PARENTHESE_OUVRANTE GUILLEMENT IDF GUILLEMENT PARENTHESE_FERMANTE {
        $$ = arbre_creer_noeud_vide(A_ECRIT);
    }
;

test_arithmetiques: PARENTHESE_OUVRANTE expression DOUBLE_EGAL expression PARENTHESE_FERMANTE {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_TEST_ARITH),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_DOUBLE_EGAL),
                arbre_concat_pere_frere($2, $4)
            )
        );
    }
	| PARENTHESE_OUVRANTE expression SUPERIEUR expression PARENTHESE_FERMANTE {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_TEST_ARITH),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_SUP),
                arbre_concat_pere_frere($2, $4)
            )
        );
    }
	| PARENTHESE_OUVRANTE expression INFERIEUR expression PARENTHESE_FERMANTE {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_TEST_ARITH),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_INF),
                arbre_concat_pere_frere($2, $4)
            )
        );
    }
	| PARENTHESE_OUVRANTE expression SUP_EGAL expression PARENTHESE_FERMANTE {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_TEST_ARITH),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_SUP_EGAL),
                arbre_concat_pere_frere($2, $4)
            )
        );
    }
	| PARENTHESE_OUVRANTE expression INF_EGAL expression PARENTHESE_FERMANTE {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_TEST_ARITH),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_INF_EGAL),
                arbre_concat_pere_frere($2, $4)
            )
        );
    }
;

resultat_retourne: {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_RESULTAT_RET),
            arbre_creer_arbre_vide(A_EXPR, 0, VALEUR_NULL)
        );
    }
    | expression {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_RESULTAT_RET),
            $1
        );
    }
;

liste_arguments: {
		$$ = arbre_creer_noeud_vide(A_LISTE_ARGS);
	}
	| PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE {
		$$ = arbre_concat_pere_fils(arbre_creer_noeud_vide(A_LISTE_ARGS), $2);
	}
;

liste_args: un_arg {
		$$ = $1;
	}
	| liste_args VIRGULE un_arg {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

un_arg: expression {
		$$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_ARG),
            $1
        );
  }
;

condition: SI expression_booleenne
	ALORS liste_instructions
	SINON liste_instructions {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_CONDITION),
			arbre_concat_pere_frere(
				arbre_concat_pere_fils(
					arbre_creer_noeud_vide(A_SI),
					$2
				),
				arbre_concat_pere_frere(
					arbre_concat_pere_fils(
						arbre_creer_noeud_vide(A_ALORS),
						$4
					),
					arbre_concat_pere_fils(
						arbre_creer_noeud_vide(A_SINON),
						$6
					)
				)
			)
		);
	}
;

tant_que: TANT_QUE expression_booleenne FAIRE liste_instructions {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_TANT_QUE),
			arbre_concat_pere_frere($2, $4)
		);
	}
;

affectation: variable OPAFF expression {
		$$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_AFFECT), 
			arbre_concat_pere_frere(
				$1,
				$3
			)
		);
	}
;

    // NOK !!!!!!!!!!!
variable: IDF {
		$$ = arbre_creer_noeud(A_VAR, yylval.t_entier, VALEUR_NULL);
	}
	| variable POINT variable {
        $$ = arbre_creer_noeud(A_VAR, $3->valeur_1, VALEUR_NULL);
	}
    | lecture_tableau {
        $$ = $1;
    }
;

appel: IDF liste_arguments {
        $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_APPEL),
            arbre_concat_pere_frere(
                arbre_creer_noeud(A_IDF, yylval.t_entier, VALEUR_NULL),
                $2
            )
		);
    }
;

lecture_tableau: IDF CROCHET_OUVRANT lecture_dimensions CROCHET_FERMANT {
    $$ = arbre_concat_pere_fils(
			arbre_creer_noeud_vide(A_LECTURE_TAB),
            arbre_concat_pere_frere(
                arbre_creer_noeud(A_IDF, yylval.t_entier, VALEUR_NULL),
                $3
		    )
		);
    }
;

lecture_dimensions: expression {
		$$ = $1;
	}
	| lecture_dimensions VIRGULE expression {
		$$ = arbre_concat_pere_frere($1, $3);
	}
;

expression: expression_arithmetiques {
		$$ = $1;
	}
	| expression_booleenne {
		$$ = $1;
	}
;

expression_arithmetiques: expression_arithmetiques PLUS exp {
        int e1 = $1->valeur_1;
        int e2 = $3->valeur_1;
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud(A_EXPR_ARITH, e1 + e2, VALEUR_NULL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_PLUS),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
	| expression_arithmetiques MOINS exp {
        int e1 = $1->valeur_1;
        int e2 = $3->valeur_1;
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud(A_EXPR_ARITH, e1 - e2, VALEUR_NULL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_MOINS),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
	| expression_arithmetiques MULT exp {
        int e1 = $1->valeur_1;
        int e2 = $3->valeur_1;
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud(A_EXPR_ARITH, e1 * e2, VALEUR_NULL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_MULT),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
	| expression_arithmetiques DIV exp {
        int e1 = $1->valeur_1;
        int e2 = $3->valeur_1;
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud(A_EXPR_ARITH, e1 / e2, VALEUR_NULL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_DIV),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
    | exp {
        $$ = $1;
    }
    | PARENTHESE_OUVRANTE expression_arithmetiques PARENTHESE_FERMANTE {
        $$ = $2;
    }
;

exp: variable {
		$$ = $1;
	}
	| ENTIER {
        $$ = arbre_creer_noeud(A_EXPR, yylval.t_entier, VALEUR_NULL);
	}
	| REEL {
		$$ = arbre_creer_noeud(A_EXPR, yylval.t_entier, VALEUR_NULL);
	}
    | PARENTHESE_OUVRANTE variable PARENTHESE_FERMANTE {
        $$ = $2;
    }
;

expression_booleenne: booleen {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_EXPR_BOOL),
            $1
        );
	}
	| booleen ET booleen {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_EXPR_BOOL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_ET),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
	| booleen OU booleen {
		$$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_EXPR_BOOL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_OU),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
	| PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE {
		$$ = $2;
	}
	| expression_booleenne ET expression_booleenne {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_EXPR_BOOL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_ET),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
	| expression_booleenne OU expression_booleenne {
        $$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_EXPR_BOOL),
            arbre_concat_pere_fils(
                arbre_creer_noeud_vide(A_OU),
                arbre_concat_pere_frere($1, $3)
            )
        );
	}
    | test_arithmetiques {
        $$ = $1;
    }
;

booleen: variable {
		$$ = $1;
	}
	| NON booleen {
		$$ = arbre_concat_pere_fils(
            arbre_creer_noeud_vide(A_NON),
            $2
        );
	}
    | VRAI {
        $$ = arbre_creer_noeud(A_EXPR, 1, VALEUR_NULL);
    }
    | FAUX {
        $$ = arbre_creer_noeud(A_EXPR, 0, VALEUR_NULL);
    }
;


%%

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(EXIT_FAILURE);
}

void usage(char *s) {
    fprintf(stderr, "Usage: %s [options] input [output]\n\tinput : fichier d'entree\n\toutput : fichier de sortie\n\n\tOPTIONS:\n\t\t-v : Affiche les tables et les arbres\n\t\t-h : Affiche l'aide\n", s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    FILE *yyout = NULL;
    int opt;
    int verbose = 0;
    int nb_flags = 0;
    while((opt = getopt(argc, argv, "vh")) != -1) {
        switch(opt){
            case 'v':
                verbose = 1;
                nb_flags++;
                break;
            case 'h':
                usage(argv[0]);
                break;
            case '?':
                usage(argv[0]);
                break;
        }
    }

    if (argc < 2 + nb_flags || argc > 3 + nb_flags) {
        usage(argv[0]);
    }

    yyin = fopen(argv[1 + nb_flags], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s en lecture.\n", argv[1 + nb_flags]);
        return EXIT_FAILURE;
    }
    
    if (argc == 3 + nb_flags) {
        yyout = fopen(argv[2 + nb_flags],"w");

        if (yyout == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier %s en ecriture.\n", argv[2 + nb_flags]);
            return EXIT_FAILURE;
        }
    }

    fprintf(stdout, "Debut de la compilation\n");

    init_compilation();

    yyparse();

	if (verbose) {
        affiche_tables();
    }

    fclose(yyin);
    if (argc == 3 + nb_flags) {
        sauvegarde_tables(yyout);
        fclose(yyout);
    }

    fprintf(stdout, "Compilation terminee\n");

	return EXIT_SUCCESS;
}
