projet: lex yacc
	cc lex.yy.c y.tab.c -o bas.exe

lex-test: lex
	cc lex.yy.c -o lex-test
	rm -f *.yy.c

lex:
	lex bas.lex

yacc:
	yacc -d bas.y

clean:
	-rm -f *.yy.c *.tab.c *.exe *-test
