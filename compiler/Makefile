
INSTALL_DIR=$(INSTALL_ROOT)/usr/bin
COPY=cp -f
MKDIR=mkdir -p
INCPATH=-I../

.PHONY: all
all: hexacompiler expcompiler

check:

hexacompiler: compiler.yy.o compiler.tab.o
	gcc $(CFLAGS) -o $@ $^

expcompiler: compilerExp.yy.o compilerExp.tab.o
	gcc $(CFLAGS) -o $@ $^


%.o: %.c
	gcc $(CFLAGS) -o $@ -c $^

%.yy.c: %.flex %.tab.h
	flex -o$@ $<

%.tab.c %.tab.h: %.y model.h
	bison -d $<


compilerExp.y: compiler.y
	sed s/main:file/main:right_value/g compiler.y > compilerExp.y

compilerExp.flex: compiler.flex
	sed s/compiler.tab.h/compilerExp.tab.h/g compiler.flex > compilerExp.flex


model.h: model ../models/hmcmodel.csv ../models/hmcoperators.csv
	./model

model: model.o ../core/util/strutil.o ../core/util/csvreader.o
	$(CXX) -DNO_VARIADIC=1 $(CXXFLAGS) $(INCPATH) -o $@ $^

%.o: %.cpp
	$(CXX) -DNO_VARIADIC=1 $(CXXFLAGS) $(INCPATH) -o $@ -c $<


.PHONY: clean
clean:
	rm -rf model model.exe model.h model.o
	rm -f hexacompiler hexacompiler.exe
	rm -f compiler.tab.c compiler.tab.o
	rm -f compiler.yy.c compiler.yy.o
	rm -f expcompiler expcompiler.exe
	rm -f compilerExp.y compilerExp.flex
	rm -f compilerExp.tab.c compilerExp.tab.o
	rm -f compilerExp.yy.c compilerExp.yy.o
	rm -f ../core/util/strutil.o ../core/util/csvreader.o

distclean: clean

install:
	$(MKDIR) $(INSTALL_DIR)
	$(COPY) expcompiler $(INSTALL_DIR)
	$(COPY) hexacompiler $(INSTALL_DIR)

uninstall:
	$(RM) $(INSTALL_DIR)/expcompiler
	$(RM) $(INSTALL_DIR)/hexacompiler
