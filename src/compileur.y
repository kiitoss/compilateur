/*
Source code: https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/genprogc/ie_prog_4lex_yacc.htm
*/
%{
  #include <stdio.h>
%}
%start programme
%token STRUCT FSTRUCT
%token SI ALORS SINON RETOURNE
%token TANT_QUE FAIRE
%token FONCTION
%token BOOLEEAN ENTIER REEL VIDE
%token CARACTERE CHAINE
%token CROCHET_OUVRANT CROCHET_FERMANT PARENTHESE_OUVRANTE PARENTHESE_FERMANTE
%token CSTE_ENTIERE
%token TABLEAU DE
%token DEBUT FIN
%token POINT_POINT POINT_VIRGULE VIRGULE DEUX_POINTS
%token DIGIT
%token TYPE IDF OPAFF
%token PROCEDURE
%token VARIABLE
%token LETTER SPACE 																// MUST BE REMOVE
/*
%token DIGIT LETTER
%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS
*/
%%
programme:                   		
        	| programme corps '\n'
            | programme error '\n'  { yyerrok; }
			| programme corps														// MUST BE REMOVE
            ;

corps:   	liste_declarations liste_instructions 	{}
         	| liste_instructions                  	{}
			| LETTER								{ printf("%c\n",'a'+$1); } 		// MUST BE REMOVE
         	;

liste_declarations:	declaration POINT_VIRGULE                      {}
                	| liste_declarations declaration POINT_VIRGULE {}
                    ;

liste_instructions:	DEBUT suite_liste_inst FIN {}
                    ;

suite_liste_inst:	instruction POINT_VIRGULE                    {}
                  	| suite_liste_inst instruction POINT_VIRGULE {}
                  	;

declaration:	declaration_type        {}
             	| declaration_variable  {}
             	| declaration_procedure {}
             	| declaration_fonction  {}
            	;

declaration_type:	TYPE IDF DEUX_POINTS suite_declaration_type {}
                	;

suite_declaration_type:	STRUCT liste_champs FSTRUCT     {}
                        | TABLEAU dimension DE nom_type {}
                        ;

dimension:	CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT {}
        	;

liste_dimensions:	une_dimension                            {}
                  	| liste_dimensions VIRGULE une_dimension {}
                  	;

une_dimension:	expression POINT_POINT expression {}
            	;

liste_champs:	un_champ                              {}
              	| liste_champs POINT_VIRGULE un_champ {}
              	;

un_champ:	IDF DEUX_POINTS nom_type {}
          	;

nom_type:	type_simple {}
        	| IDF       {}
	  		;

type_simple: 	ENTIER                                                {}
             	| REEL                                                {}
             	| BOOLEEAN                                            {}
             	| CARACTERE                                           {}
             	| CHAINE CROCHET_OUVRANT CSTE_ENTIERE CROCHET_FERMANT {}
             	;

declaration_variable:	VARIABLE IDF DEUX_POINTS nom_type {}
                      	;

declaration_procedure:	PROCEDURE IDF liste_parametres RETOURNE type_simple corps {}
                       	;

declaration_fonction:	FONCTION IDF liste_parametres RETOURNE type_simple corps {}
                    	;

liste_parametres:                                                       {}
                	| PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE {}
                	;

liste_param:	un_param                             {}
            	| liste_param POINT_VIRGULE un_param {}
            	;

un_param:	IDF DEUX_POINTS type_simple {}
        	;

instruction:	affectation                  {}
             	| condition                  {}
             	| tant_que                   {}
             	| appel                      {}
             	| VIDE                       {}
             	| RETOURNE resultat_retourne {}
             	;

resultat_retourne:              {}
                	| expression {}
                   	;

appel:	IDF liste_arguments {}
       ;	

liste_arguments:                                                      {}
                	| PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE {}
                	;

liste_args:	un_arg                      {}
            | liste_args VIRGULE un_arg {}
            ;

un_arg:	expression {}
    	;

condition:	SI expression_booleenne ALORS liste_instructions SINON liste_instructions {}
           	;

tant_que:	TANT_QUE expression_booleenne FAIRE liste_instructions {}
          	;

affectation:	variable OPAFF expression {}
             	;

variable:	DIGIT {}
          	;

expression:	VARIABLE OPAFF VARIABLE {}
			;
expression_booleenne:	VARIABLE OPAFF VARIABLE {}
                      	;
%%

int main(int argc, char *argv[]) {
	return(yyparse());
}

void yyerror(char *s) {
	fprintf(stderr, "Erreur: %s\n", s);
}

int yywrap() {
	return(1);
}
