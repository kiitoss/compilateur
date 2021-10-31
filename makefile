CC = gcc
CFLAGS = -W -Wall

compilateur-clean: compilateur simple-clean

compilateur: cpyrr-lex cpyrr-yacc tlex.o
	$(CC) $(CFLAGS) lex.yy.c y.tab.c tlex.o -o cpyrr.exe

cpyrr-lex: src/cpyrr.l
	lex src/cpyrr.l

cpyrr-yacc: src/cpyrr.y
	yacc -d src/cpyrr.y


tdec.o: src/tdec.c src/inc/tdec.h
	$(CC) $(CFLAGS) src/tdec.c -c

tlex.o: src/tlex.c src/inc/tlex.h
	$(CC) $(CFLAGS) src/tlex.c -c

arbre.o: src/arbre.c src/inc/arbre.h
	$(CC) $(CFLAGS) src/arbre.c -c


test: arbre.test tlex.test tdec.test

tdec.test: tdec.test.o tdec.o
	$(CC) $(CFLAGS) tdec.test.o tdec.o -o tdec.test.exe

tdec.test.o: src/tests/tdec.test.c
	$(CC) $(CFLAGS) src/tests/tdec.test.c -c

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
