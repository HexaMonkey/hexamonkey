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
	
installer.msi: installer.wixobj gui/release/hexamonkey.exe dll/Qt5Core.dll dll/Qt5Gui.dll dll/Qt5Widgets.dll dll/libgcc_s_sjlj-1.dll dll/libstdc++-6.dll compiler/hexacompiler.exe compiler/expcompiler.exe  core/modules/hmc/hmcmodel.csv core/modules/mkv/mkvmodel.xml core/modules/mp4/mp4model.csv scripts/avi.hm scripts/asf.hm scripts/microsoft.hm
	light installer.wixobj
	
installer.wixobj: installer.wxs
	candle installer.wxs
	
.PHONY: clean	
clean:
	rm hexamonkey  hexacompiler  expcompiler; cd compiler; make clean
	
	
