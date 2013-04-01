CC=gcc
QMAKE=qmake
BUILD_DIR=./gui/release
JOBS=-j5

hexamonkey: $(BUILD_DIR)/hexamonkey	hexacompiler expcompiler hmcmodel.csv mp4model.csv mkvmodel.xml
	cp $(BUILD_DIR)/hexamonkey* .

.PHONY: $(BUILD_DIR)/hexamonkey	
$(BUILD_DIR)/hexamonkey:
	mkdir -p $(BUILD_DIR); $(QMAKE) ./gui/gui.pro -r -o $(BUILD_DIR)/Makefile ; cd $(BUILD_DIR); make $(JOBS)
	 
hmcmodel.csv: core/modules/hmc/hmcmodel.csv
	cp core/modules/hmc/hmcmodel.csv .
	
mp4model.csv: core/modules/mp4/mp4model.csv
	cp core/modules/mp4/mp4model.csv .

mkvmodel.xml: core/modules/mkv/mkvmodel.xml
	cp core/modules/mkv/mkvmodel.xml .

hexacompiler: compiler/hexacompiler
	cp compiler/hexacompiler .
	
expcompiler: compiler/expcompiler
	cp compiler/expcompiler .
	
.PHONY: compiler/hexacompiler		
compiler/hexacompiler:
	cd compiler; make CFLAGS=$(CFLAGS) hexacompiler
	
.PHONY: compiler/expcompiler
compiler/expcompiler:
	cd compiler; make CFLAGS=$(CFLAGS) expcompiler
	
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
	
.PHONY: clean	
clean:
	rm hexamonkey  hexacompiler  expcompiler; cd compiler; make clean
	
	
