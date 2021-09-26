projet-clean: projet simple-clean

projet: lex yacc
	cc lex.yy.c y.tab.c -o bas.exe

lex-test: lex
	cc lex.yy.c -o lex-test
	rm -f *.yy.c

lex:
	lex src/bas.lex

yacc:
	yacc -d src/bas.yacc

simple-clean:
	rm -f *.yy.c *.tab.c *-test *.tab.h

clean: simple-clean
	rm -f *.exe
