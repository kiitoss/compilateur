CC = gcc
CFLAGS = -W -Wall

compileur-clean: compileur simple-clean

compileur: compileur-lex compileur-yacc
	cc lex.yy.c y.tab.c -o compileur.exe

compileur-lex:
	lex src/compileur.l

compileur-yacc:
	yacc -d src/compileur.y


fsem.o: src/fsem.c
	$(CC) $(CFLAGS) src/fsem.c -c


test: fsem.test.o fsem.o
	$(CC) $(CFLAGS) fsem.test.o fsem.o -o fsem.test.exe

fsem.test.o: src/tests/fsem.test.c
	$(CC) $(CFLAGS) src/tests/fsem.test.c -c


simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o *.test.exe

clean: simple-clean
	rm -f *.exe
