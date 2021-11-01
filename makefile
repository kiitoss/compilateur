CC = gcc
CFLAGS = -W -Wall

compilateur-clean: compilateur simple-clean

compilateur: cpyrr-lex cpyrr-yacc tlex.o tdec.o trep.o treg.o global.o
	$(CC) lex.yy.c y.tab.c tlex.o tdec.o trep.o treg.o global.o -o cpyrr.exe

cpyrr-lex: src/cpyrr.l
	lex src/cpyrr.l

cpyrr-yacc: src/cpyrr.y
	yacc -d src/cpyrr.y


global.o: src/global.c src/inc/global.h
	$(CC) $(CFLAGS) src/global.c -c

treg.o: src/treg.c src/inc/treg.h
	$(CC) $(CFLAGS) src/treg.c -c

trep.o: src/trep.c src/inc/trep.h
	$(CC) $(CFLAGS) src/trep.c -c

tdec.o: src/tdec.c src/inc/tdec.h
	$(CC) $(CFLAGS) src/tdec.c -c

tlex.o: src/tlex.c src/inc/tlex.h
	$(CC) $(CFLAGS) src/tlex.c -c

arbre.o: src/arbre.c src/inc/arbre.h
	$(CC) $(CFLAGS) src/arbre.c -c


test: arbre.test tlex.test tdec.test trep.test treg.test

treg.test: treg.test.o treg.o arbre.o
	$(CC) $(CFLAGS) treg.test.o treg.o arbre.o -o treg.test.exe

treg.test.o: src/tests/treg.test.c
	$(CC) $(CFLAGS) src/tests/treg.test.c -c

trep.test: trep.test.o trep.o global.o
	$(CC) $(CFLAGS) trep.test.o trep.o global.o -o trep.test.exe

trep.test.o: src/tests/trep.test.c
	$(CC) $(CFLAGS) src/tests/trep.test.c -c

tdec.test: tdec.test.o tdec.o global.o
	$(CC) $(CFLAGS) tdec.test.o tdec.o global.o -o tdec.test.exe

tdec.test.o: src/tests/tdec.test.c
	$(CC) $(CFLAGS) src/tests/tdec.test.c -c

tlex.test: tlex.test.o tlex.o global.o
	$(CC) $(CFLAGS) tlex.test.o tlex.o global.o -o tlex.test.exe

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
