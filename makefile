CC = gcc
CFLAGS = -W -Wall

compileur-clean: compileur simple-clean

compileur: compileur-lex compileur-yacc
	cc lex.yy.c y.tab.c -o compileur.exe

compileur-lex:
	lex src/compileur.l

compileur-yacc:
	yacc -d src/compileur.y


tlex.o: src/tlex.c
	$(CC) $(CFLAGS) src/tlex.c -c

arbre.o: src/arbre.c
	$(CC) $(CFLAGS) src/arbre.c -c


test: arbre.test tlex.test

tlex.test: tlex.test.o tlex.o
	$(CC) $(CFLAGS) tlex.test.o tlex.o -o tlex.test.exe

tlex.test.o: src/tests/tlex.test.c
	$(CC) $(CFLAGS) src/tests/tlex.test.c -c

arbre.test: arbre.test.o arbre.o
	$(CC) $(CFLAGS) arbre.test.o arbre.o -o arbre.test.exe

arbre.test.o: src/tests/arbre.test.c
	$(CC) $(CFLAGS) src/tests/arbre.test.c -c


simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o *.test.exe

clean: simple-clean
	rm -f *.exe
