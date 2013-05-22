CC=gcc
QMAKE=qmake
ifeq ("$(shell which $(QMAKE) 2>/dev/null)","")
QMAKE=qmake-qt5
endif

TMP:=$(shell $(QMAKE) --version |grep "Qt version 5")
ifeq ("$(TMP)","")
$(error you need QT5 to build)
endif

BUILD_DIR:=./gui/release
INSTALL_DIR:=./gui/prefix
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

$(MAKEFILE): ./gui/gui.pro compiler $(BUILD_DIR_EXISTS)
	# stupid qmake-qt5 puts the Makefile in gui/release/gui/release/Makefile so I use redirects instead
	# $(QMAKE) $< -o $@
	# also I have to run it from the $(BUILD_DIR) directory
	cd $(BUILD_DIR); $(QMAKE) ../gui.pro -o - > Makefile

$(PROGRAM): $(MAKEFILE)
	make -C $(BUILD_DIR) $(JOBS)

compiler:
	make -C compiler

install: $(INSTALL_DIR)/$(EXISTS_FILE)
	cp -rf scripts/ $(INSTALL_DIR)
	cp $(PROGRAM) $(INSTALL_DIR)
	cp core/modules/hmc/hmcmodel.csv $(INSTALL_DIR)
	cp core/modules/mkv/mkvmodel.xml $(INSTALL_DIR)
	cp compiler/hexacompiler $(INSTALL_DIR)
	cp compiler/expcompiler $(INSTALL_DIR)
	cp gui/logo.svg $(INSTALL_DIR)
	@echo "***********************************************"
	@echo "* hexamonkey is installed                     *"
	@echo "* type cd $(INSTALL_DIR); ./hexamonkey to run it*"
	@echo "***********************************************"
	
installer.msi: installer.wixobj\
 gui/release/hexamonkey.exe\
 compiler/hexacompiler.exe\
 compiler/expcompiler.exe\
 core/modules/hmc/hmcmodel.csv\
 core/modules/mkv/mkvmodel.xml\
 scripts/avi.hm\
 scripts/asf.hm\
 scripts/microsoft.hm\
 scripts/ts.hm
 
	light installer.wixobj
	
installer.wixobj: installer.wxs
	candle installer.wxs
	
.PHONY: clean compiler all

distclean:
	rm -rf $(INSTALL_DIR)
	
clean:
	rm -rf $(BUILD_DIR)
	
