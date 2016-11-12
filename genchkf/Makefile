genchkf: parser lexer hashstring ast symtab pass
	gcc lex.yy.c genchkf.tab.c hashstring.o ast.o symtab.o pass.o -o genchkf

lexer:
	flex genchkf.lex

parser:
	bison -dv genchkf.y

hashstring:
	gcc -c hashstring.c -o hashstring.o

ast:
	gcc -c ast.c -o ast.o

symtab:
	gcc -c symtab.c -o symtab.o

pass:
	gcc -c pass.c -o pass.o

test:
	./genchkf < f.stp && echo "pass."

clean:
	rm -f genchkf genchkf.tab.h genchkf.tab.c genchkf.output lex.yy.c *.o *~
