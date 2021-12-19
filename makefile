CC = gcc
CFLAGS = -W -Wall -pedantic -std=c99 -O3

compilateur-clean: compilateur simple-clean

compilateur: cpyrr-lex cpyrr-yacc pile.o thash.o tlex.o tdec.o trep.o treg.o utils.o
	$(CC) lex.yy.c y.tab.c pile.o thash.o tlex.o tdec.o trep.o treg.o utils.o -o cpyrr.exe

cpyrr-lex: src/cpyrr.l
	lex src/cpyrr.l

cpyrr-yacc: src/cpyrr.y
	yacc -d src/cpyrr.y


utils.o: src/utils.c inc/utils.h inc/global.h
	$(CC) $(CFLAGS) src/utils.c -c

treg.o: src/tables/treg.c inc/treg.h
	$(CC) $(CFLAGS) src/tables/treg.c -c

trep.o: src/tables/trep.c inc/trep.h inc/global.h
	$(CC) $(CFLAGS) src/tables/trep.c -c

tdec.o: src/tables/tdec.c inc/tdec.h inc/global.h
	$(CC) $(CFLAGS) src/tables/tdec.c -c

tlex.o: src/tables/tlex.c inc/tlex.h inc/thash.h inc/global.h
	$(CC) $(CFLAGS) src/tables/tlex.c -c

thash.o: src/tables/thash.c inc/thash.h
	$(CC) $(CFLAGS) src/tables/thash.c -c

pile.o: src/tad/pile.c inc/pile.h
	$(CC) $(CFLAGS) src/tad/pile.c -c

simple-clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o

clean: simple-clean
	rm -f *.exe
