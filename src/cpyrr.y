%{
	#include "inc/tlex.h"
	#include "inc/tdec.h"	

	#define AFFICHER_TABLES 1
	#define AFFICHER_ARBRE 0
	
	int yylex(void);
	void yyerror(char *);
	
	int line;

    int global_index_tlex = 0;
    int global_nb_champs = 0;
    int global_tab_taille = 0;
    int global_index_tdec = 0;
    int global_num_region = 0;

    void maj_index_tlex(int index_tlex);
    int recupere_index_tlex();
    int recupere_num_region();
    int recupere_nb_champs();
    void init_nb_champs();
    void incr_nb_champs();
    int recupere_index_trep();
    void init_tab_taille();
    int recupere_taille_tab();
    void maj_index_tdec();
    int recupere_index_tdec();
    int preg_nouvelle_region();
    void maj_num_region(int num_region);
%}

%union {
	// arbre t_arbre;
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

%token <t_entier> IDF ENTIER BOOLEEN REEL CARACTERE CSTE_FORMAT

// Tokens provisoires
%token AFFICHE


/* %type <t_arbre> corps liste_declarations liste_instructions suite_liste_inst declaration
%type <t_arbre> declaration_variable declaration_fonction declaration_procedure declaration_type
%type <t_arbre> suite_declaration_type liste_champs nom_type dimension liste_dimensions une_dimension
%type <t_arbre> expression un_champ liste_parametres liste_param un_param liste_arguments liste_args un_arg
%type <t_arbre> instruction condition tant_que affectation
%type <t_arbre> expression_booleenne tableau variable appel */

%type <t_entier> type_simple booleen expression_arithmetiques exp 


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
declaration_type: TYPE IDF { maj_index_tlex($2); } DEUX_POINTS suite_declaration_type {
        
	}
;
	/* La suite de déclaration d'un type est soit une structure, soit un tableau */
suite_declaration_type: STRUCT { init_nb_champs(); } liste_champs FSTRUCT {
        tdec_nouvelle_entree(recupere_index_tlex(), TYPE_S, recupere_num_region(), recupere_index_trep(), recupere_nb_champs());
	}
	| TABLEAU { init_tab_taille(); } dimension DE nom_type {
        tdec_nouvelle_entree(recupere_index_tlex(), TYPE_T, recupere_num_region(), recupere_index_trep(), recupere_taille_tab());
	}
;

	/* Les dimensions d'un tableau sont représentées entre crochet */
dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT {
	}
;

liste_dimensions: une_dimension {
	}
	| liste_dimensions VIRGULE une_dimension {
	}
;

une_dimension: expression POINT_POINT expression {
	}
;

liste_champs: un_champ {
        incr_nb_champs();
	}
	| liste_champs POINT_VIRGULE un_champ {
	}
;

un_champ: IDF DEUX_POINTS nom_type {
	}
;

	/* Le type peut être un type simple (entier, reel...) ou un identificateur (variable...) */
nom_type: type_simple {
	}
	| IDF { maj_index_tdec($1); }
;

	/** A modifier */
type_simple: ENTIER { maj_index_tdec(0); }
	| REEL { maj_index_tdec(1); }
	| BOOLEEN { maj_index_tdec(2); }
	| CARACTERE { maj_index_tdec(3); }
;


	/* Grammaire de déclaration d'une variable */
declaration_variable: VARIABLE IDF DEUX_POINTS nom_type {
        tdec_nouvelle_entree($2, VAR, recupere_num_region(), recupere_index_tdec(), 0);
    }
;


	/* Grammaire de déclaration d'une fonction */
declaration_fonction: FONCTION IDF {
        int num_region_parente = recupere_num_region();
        int num_region = preg_nouvelle_region();
        maj_num_region(num_region);
        tdec_nouvelle_entree($2, FONC, num_region_parente, recupere_index_trep(), num_region);
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
instruction: AFFICHE expression {
	}
	| condition {
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
	}
	| REEL {
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

/* Insere les types de base dans les tables */
void insere_types_base() {
    int index_tlex_entier = tlex_insere("entier");
    int index_tlex_reel = tlex_insere("reel");
    int index_tlex_booleen = tlex_insere("booleen");
    int index_tlex_caractere = tlex_insere("caractere");
    tdec_nouvelle_entree(index_tlex_entier, 0, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_reel, 0, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_booleen, 0, 0, VALEUR_NULL, 1);
    tdec_nouvelle_entree(index_tlex_caractere, 0, 0, VALEUR_NULL, 1);
}

void maj_index_tlex(int index_tlex) { global_index_tlex = index_tlex; }

int recupere_index_tlex() { return global_index_tlex; }

int recupere_num_region() { return 0; }

void init_nb_champs() { global_nb_champs = 0; }

void incr_nb_champs() { global_nb_champs++; }

int recupere_nb_champs() { return global_nb_champs; }

int recupere_index_trep() { return 0; }

void init_tab_taille() { global_tab_taille = 0; }

int recupere_taille_tab() { return global_tab_taille; }

void maj_index_tdec(int index_tdec) { global_index_tdec = index_tdec; }

int recupere_index_tdec() { return global_index_tdec; }

void maj_num_region(int num_region) { global_num_region = num_region; }

int preg_nouvelle_region() { return 0; }

void yyerror(char *s) {
	fprintf(stderr, "ligne %d: %s\n", line, s);
	exit(-1);
}


int main(void) {
    init_thash();
    tdec_init();

    insere_types_base();

	yyparse();

	if (AFFICHER_TABLES) {
		/* printf("\n\nAffichage de la table de hash-code:\n");
		thash_affiche();
		printf("\n\nAffichage de la table lexicographique:\n");
		tlex_affiche(); */
		printf("\n\nAffichage de la table des déclarations:\n");
		tdec_affiche();
		printf("\n");
	} else {
		printf("Affichage des tables désactivé.\n");
	}
	return 0;
}
