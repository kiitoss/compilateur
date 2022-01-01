CC = gcc
CFLAGS = -W -Wall -pedantic -std=c99 -O3

tables = thash.o tlex.o tdec.o trep.o treg.o tables.o
utils = variable.o structure.o tableau.o fonc_proc.o utils.o

all: interpreteur compilateur simple-clean

interpreteur: interpreteur-lex interpreteur-yacc pile.o $(tables)
	$(CC) lex.yy.c y.tab.c pile.o $(tables) -o interpreteur.exe

interpreteur-lex: src/interpreteur/interpreteur.l
	lex src/interpreteur/interpreteur.l

interpreteur-yacc: src/interpreteur/interpreteur.y
	yacc -d src/interpreteur/interpreteur.y




compilateur: compilateur-lex compilateur-yacc arbre.o pile.o $(tables) $(utils)
	$(CC) lex.yy.c y.tab.c arbre.o pile.o $(tables) $(utils) -o compilateur.exe

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


# TABLES
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

# TAD
arbre.o: src/tad/arbre.c inc/arbre.h
	$(CC) $(CFLAGS) src/tad/arbre.c -c

pile.o: src/tad/pile.c inc/pile.h
	$(CC) $(CFLAGS) src/tad/pile.c -c


simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o

clean: simple-clean
	rm -f *.exe
