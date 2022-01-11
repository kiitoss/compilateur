CC = gcc
CFLAGS = -W -Wall -pedantic -std=c99 -O3

tables = thash.o tlex.o tdec.o trep.o treg.o tables.o
utils-compilateur = variable.o structure.o tableau.o fonc_proc.o utils.o
utils-interpreteur = appel.o expression-booleenne.o expression.o operation.o variable-interpreteur.o ecriture.o lecture.o affectation.o

all: interpreteur compilateur simple-clean

interpreteur-clean: interpreteur simple-clean

compilateur-clean: compilateur simple-clean


###########################################
#               INTERPRETEUR              #
###########################################

interpreteur: interpreteur-lex interpreteur-yacc arbre.o pile.o pexec.o $(tables) ${utils-interpreteur} executeur.o
	$(CC) lex.yy.c y.tab.c arbre.o pile.o pexec.o $(tables) ${utils-interpreteur} executeur.o -o interpreteur.exe

interpreteur-lex: src/interpreteur/interpreteur.l
	lex src/interpreteur/interpreteur.l

interpreteur-yacc: src/interpreteur/interpreteur.y
	yacc -d src/interpreteur/interpreteur.y

executeur.o: src/interpreteur/executeur.c
	$(CC) $(CFLAGS) src/interpreteur/executeur.c -c

pexec.o: src/tad/pexec.c
	$(CC) $(CFLAGS) src/tad/pexec.c -c

appel.o: src/interpreteur/utils/appel.c
	$(CC) $(CFLAGS) src/interpreteur/utils/appel.c -c

expression-booleenne.o: src/interpreteur/utils/expression-booleenne.c
	$(CC) $(CFLAGS) src/interpreteur/utils/expression-booleenne.c -c

expression.o: src/interpreteur/utils/expression.c
	$(CC) $(CFLAGS) src/interpreteur/utils/expression.c -c

operation.o: src/interpreteur/utils/operation.c
	$(CC) $(CFLAGS) src/interpreteur/utils/operation.c -c

variable-interpreteur.o: src/interpreteur/utils/variable-interpreteur.c
	$(CC) $(CFLAGS) src/interpreteur/utils/variable-interpreteur.c -c

ecriture.o: src/interpreteur/utils/ecriture.c
	$(CC) $(CFLAGS) src/interpreteur/utils/ecriture.c -c

lecture.o: src/interpreteur/utils/lecture.c
	$(CC) $(CFLAGS) src/interpreteur/utils/lecture.c -c

affectation.o: src/interpreteur/utils/affectation.c
	$(CC) $(CFLAGS) src/interpreteur/utils/affectation.c -c


###########################################
#               COMPILATEUR               #
###########################################

compilateur: compilateur-lex compilateur-yacc arbre.o pile.o $(tables) $(utils-compilateur)
	$(CC) lex.yy.c y.tab.c arbre.o pile.o $(tables) $(utils-compilateur) -o compilateur.exe

compilateur-lex: src/compilateur/compilateur.l
	lex src/compilateur/compilateur.l

compilateur-yacc: src/compilateur/compilateur.y
	yacc -d src/compilateur/compilateur.y

# UTILS (COMPILATEUR)
utils.o: src/compilateur/utils/utils.c inc/compilateur.h
	$(CC) $(CFLAGS) src/compilateur/utils/utils.c -c

fonc_proc.o: src/compilateur/utils/fonc_proc.c inc/compilateur.h
	$(CC) $(CFLAGS) src/compilateur/utils/fonc_proc.c -c

tableau.o: src/compilateur/utils/tableau.c inc/compilateur.h
	$(CC) $(CFLAGS) src/compilateur/utils/tableau.c -c

structure.o: src/compilateur/utils/structure.c inc/compilateur.h
	$(CC) $(CFLAGS) src/compilateur/utils/structure.c -c

variable.o: src/compilateur/utils/variable.c inc/compilateur.h
	$(CC) $(CFLAGS) src/compilateur/utils/variable.c -c


###########################################
#                  TABLES                 #
###########################################
tables.o: src/tables/tables.c inc/treg.h inc/tables.h
	$(CC) $(CFLAGS) src/tables/tables.c -c

treg.o: src/tables/treg.c inc/treg.h inc/tables.h
	$(CC) $(CFLAGS) src/tables/treg.c -c

trep.o: src/tables/trep.c inc/trep.h inc/tables.h
	$(CC) $(CFLAGS) src/tables/trep.c -c

tdec.o: src/tables/tdec.c inc/tdec.h inc/tables.h
	$(CC) $(CFLAGS) src/tables/tdec.c -c

tlex.o: src/tables/tlex.c inc/tlex.h inc/thash.h inc/tables.h
	$(CC) $(CFLAGS) src/tables/tlex.c -c

thash.o: src/tables/thash.c inc/thash.h
	$(CC) $(CFLAGS) src/tables/thash.c -c

###########################################
#                   TAD                   #
###########################################
arbre.o: src/tad/arbre.c inc/arbre.h
	$(CC) $(CFLAGS) src/tad/arbre.c -c

pile.o: src/tad/pile.c inc/pile.h
	$(CC) $(CFLAGS) src/tad/pile.c -c


simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o

clean: simple-clean
	rm -f *.exe
