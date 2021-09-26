calculator-clean: calculator simple-clean

calculator: calculator-lex calculator-yacc
	cc lex.yy.c y.tab.c -o bas.exe

calculator-lex:
	lex src/bas.lex

calculator-yacc:
	yacc -d src/bas.yacc




compileur-yacc:
	yacc -d src/compileur.yacc




simple-clean:
	rm -f *.yy.c *.tab.c *-test *.tab.h

clean: simple-clean
	rm -f *.exe
