calculator-clean: calculator simple-clean

calculator: calculator-lex calculator-yacc
	cc lex.yy.c y.tab.c -o bas.exe

calculator-lex:
	lex src/bas.l

calculator-yacc:
	yacc -d src/bas.y



compileur-clean: compileur simple-clean

compileur: compileur-lex compileur-yacc
	cc lex.yy.c y.tab.c -o compileur.exe

compileur-lex:
	lex -d src/compileur.l

compileur-yacc:
	yacc -d src/compileur.y




simple-clean:
	rm -f *.yy.c *.tab.c *-test *.tab.h

clean: simple-clean
	rm -f *.exe
