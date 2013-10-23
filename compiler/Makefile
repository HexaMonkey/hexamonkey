
INCPATH=-I../

.PHONY: all
all: hexacompiler expcompiler


hexacompiler: compiler.yy.o compiler.tab.o
	$(CC) $(CFLAGS) -o $@ $^

expcompiler: compilerExp.yy.o compilerExp.tab.o
	$(CC) $(CFLAGS) -o $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.yy.c: %.flex %.tab.h
	flex -o $@ $<

%.tab.c %.tab.h: %.y model.h
	bison --warnings=none -d $<


compilerExp.y: compiler.y
	sed s/main:file/main:right_value/g compiler.y > compilerExp.y

compilerExp.flex: compiler.flex
	sed s/compiler.tab.h/compilerExp.tab.h/g compiler.flex > compilerExp.flex


model.h: model ../models/hmcmodel.csv ../models/hmcoperators.csv
	./model

model: model.o ../core/util/strutil.o ../core/util/csvreader.o
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ -c $<


.PHONY: clean
clean:
	rm -f *.o *.exe compilerExp.* compiler.tab.* lex.yy.* lexExp.yy.* model model.h hexacompiler expcompiler

distclean: clean
