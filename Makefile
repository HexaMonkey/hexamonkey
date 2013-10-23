CC=gcc
QMAKE=qmake
ifeq ("$(shell which $(QMAKE) 2>/dev/null)","")
QMAKE=qmake-qt5
endif

TMP:=$(shell $(QMAKE) --version |grep "Qt version 5")
ifeq ("$(TMP)","")
$(error you need QT5 to build)
endif

BUILD_DIR:=./objs
INSTALL_DATADIR:=/usr/local/share/hexamonkey
INSTALL_BINDIR:=/usr/local/bin
JOBS:=-j5
EXISTS_FILE:=.exists
BUILD_DIR_EXISTS:=$(BUILD_DIR)/$(EXISTS_FILE)
MAKEFILE:=$(BUILD_DIR)/Makefile
PROGRAM:=$(BUILD_DIR)/hexamonkey
HMCMODEL:=$(BUILD_DIR)/hmcmodel.csv
MKVMODEL:=$(BUILD_DIR)/mkvmodel.xml

all: $(PROGRAM)	 

%/$(EXISTS_FILE):
	mkdir -p $*

$(MAKEFILE): ./gui/gui.pro compiler core $(BUILD_DIR_EXISTS)
# stupid qmake-qt5 puts the Makefile in gui/release/gui/release/Makefile so I use redirects instead
# $(QMAKE) $< -o $@
# also I have to run it from the $(BUILD_DIR) directory
	cd $(BUILD_DIR); $(QMAKE) ../gui/gui.pro -o - > Makefile

$(PROGRAM): $(MAKEFILE) logo.svg
	make -C $(BUILD_DIR) $(JOBS)

logo.svg:
	cp media/logo.svg $(BUILD_DIR)

compiler:
	make -C compiler

core:
	cd core; qmake && make

uninstall:
	rm -rf $(INSTALL_DATADIR)
	rm $(INSTALL_BINDIR)/hexamonkey

install:
	mkdir -p $(INSTALL_DATADIR)
	mkdir -p $(INSTALL_BINDIR)
	cp -rf scripts/ $(INSTALL_DATADIR)
	cp $(PROGRAM) $(INSTALL_BINDIR)
	cp models/hmcmodel.csv $(INSTALL_DATADIR)
	cp models/mkvmodel.xml $(INSTALL_DATADIR)
	cp compiler/hexacompiler $(INSTALL_DATADIR)
	cp compiler/expcompiler $(INSTALL_DATADIR)
	cp gui/logo.svg $(INSTALL_DATADIR)
	
installer.msi: installer.wixobj\
 gui/release/hexamonkey.exe\
 compiler/hexacompiler.exe\
 compiler/expcompiler.exe\
 models/hmcmodel.csv\
 models/mkvmodel.xml\
 scripts/avi.hm\
 scripts/asf.hm\
 scripts/microsoft.hm\
 scripts/ts.hm
 
	light installer.wixobj
	
installer.wixobj: installer.wxs
	candle installer.wxs

.PHONY: clean compiler core all

distclean:
	rm -rf $(INSTALL_DIR)
	
clean:
	make -C compiler clean
	rm -rf $(BUILD_DIR)
	
