CC = gcc
CFLAGS = -W -Wall

compilateur-clean: compilateur simple-clean

compilateur: cpyrr-lex cpyrr-yacc tlex.o tdec.o global.o
	$(CC) lex.yy.c y.tab.c tlex.o tdec.o global.o -o cpyrr.exe

cpyrr-lex: src/cpyrr.l
	lex src/cpyrr.l

cpyrr-yacc: src/cpyrr.y
	yacc -d src/cpyrr.y


global.o: src/global.c inc/global.h
	$(CC) $(CFLAGS) src/global.c -c

tdec.o: src/tdec.c inc/tdec.h inc/global.h
	$(CC) $(CFLAGS) src/tdec.c -c

tlex.o: src/tlex.c inc/tlex.h inc/global.h
	$(CC) $(CFLAGS) src/tlex.c -c


simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o

clean: simple-clean
	rm -f *.exe
