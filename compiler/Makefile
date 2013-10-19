CC=gcc $(CFLAGS)

.PHONY: all	
all: hexacompiler expcompiler

hexacompiler: lex.yy.o compiler.tab.o
	gcc -o hexacompiler $^ $(CFLAGS)
	
compiler.tab.h: compiler.y ast.h model.h write.h
	bison -d compiler.y

compiler.tab.c: compiler.y ast.h model.h write.h
	bison -d compiler.y

lex.yy.c: compiler.flex compiler.tab.h
	flex compiler.flex
	
expcompiler: lexExp.yy.o compilerExp.tab.o
	gcc -o expcompiler $^ $(CFLAGS)

compilerExp.tab.h: compilerExp.y ast.h model.h write.h
	bison --warnings=none --output=compilerExp.tab.c --defines=compilerExp.tab.h compilerExp.y 2>/dev/null
compilerExp.tab.c: compilerExp.y ast.h model.h write.h
	bison --warnings=none --output=compilerExp.tab.c --defines=compilerExp.tab.h compilerExp.y 2>/dev/null

lexExp.yy.c: compilerExp.flex compilerExp.tab.h
	flex -o lexExp.yy.c compilerExp.flex

compilerExp.y: compiler.y
	sed s/main:file/main:right_value/g compiler.y > compilerExp.y
	
compilerExp.flex: compiler.flex
	sed s/compiler.tab.h/compilerExp.tab.h/g compiler.flex > compilerExp.flex
	
model.h: model ../models/hmcmodel.csv ../models/hmcoperators.csv
	./model
	
model: model.o strutil.o csvreader.o
	g++ -B ../core/util/ -o $@ $^
	
strutil.o : ../core/util/strutil.cpp ../core/util/strutil.h
	g++ -o $@ -c ../core/util/strutil.cpp

csvreader.o : ../core/util/csvreader.cpp ../core/util/csvreader.h
	g++ -o $@ -c ../core/util/csvreader.cpp
	
.PHONY: clean	
clean:
	rm -f *.o *.exe compilerExp.* compiler.tab.* lex.yy.* lexExp.yy.*

	
