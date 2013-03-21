#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T17:52:05
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG   += c++11 config

TARGET = HexaMonkey
TEMPLATE = app

INCLUDEPATH += ../core \
               ../core/modules/mp4 \
               ../core/modules/ebml \
               ../core/modules/mkv \
               ../core/modules/hmc \
               ../core/modules/ts \
			   ../core/formatdetector \
               ../core/modules/standard \
			   ../core/modules/default \
               ../core/util \
               ../core/interpreter \
               ../compiler \
               tree \
               hex \
               mt

SOURCES += main.cpp\
        mainwindow.cpp \
    ../core/object.cpp \
    ../core/file.cpp \
    ../core/variant.cpp \
    ../core/objecttypetemplate.cpp \
    ../core/objecttype.cpp \
    ../core/module.cpp \
    ../core/moduleloader.cpp \
    ../core/modules/mp4/mp4module.cpp \
    ../core/modules/standard/standardmodule.cpp \
    ../core/util/strutil.cpp \
    ../core/util/csvreader.cpp \
    tree/htmldelegate.cpp \
    tree/treewidget.cpp \
    tree/treeview.cpp \
    tree/treemodel.cpp \
    tree/treeobjectitem.cpp \
	tree/treeitem.cpp \
	tree/filterwidget.cpp \
    hex/hexfilewidget.cpp \
    hex/hexfileview.cpp \
    hex/hexfilemodel.cpp \
    hex/hexfiledockwidget.cpp \
    hex/hexfiledelegate.cpp \
	hex/hexfileheader.cpp \
	hex/hexfilesearchwidget.cpp \
    ../core/modules/mkv/mkvmodule.cpp \
    ../core/util/bitutil.cpp \
    ../core/modules/ts/tsmodule.cpp \
    ../core/modules/ebml/ebmlmodule.cpp \
    ../core/modules/hmc/hmcmodule.cpp \
    ../core/showcase.cpp \
    ../core/interpreter/interpreter.cpp \
    ../core/interpreter/filter.cpp \
    ../core/interpreter/variable.cpp \
    ../core/parser.cpp \
    ../core/modules/default/fileparser.cpp \
    ../core/modules/standard/bitparser.cpp \
    ../core/modules/standard/floatparser.cpp \
    ../core/modules/standard/intparser.cpp \
    ../core/modules/ts/tsfileparser.cpp \
    ../core/containerparser.cpp \
    ../core/modules/ts/tsadaptationfieldparser.cpp \
    ../core/modules/ts/tspacketparser.cpp \
    ../core/modules/ebml/ebmlcontainerparser.cpp \
    ../core/util/iterutil.cpp \
    ../core/modules/ebml/ebmlsimpleparser.cpp \
    ../core/modules/ebml/ebmllargeintegerparser.cpp \
    ../core/modules/ebml/ebmlmasterparser.cpp \
    ../core/modules/mp4/mp4boxparser.cpp \
    ../core/mapmodule.cpp \
    ../core/modules/standard/wordparser.cpp \
    ../core/modules/default/arrayparser.cpp \
    ../core/modules/ebml/ebmldateparser.cpp \
    ../core/interpreter/program.cpp \
    ../core/modules/ts/tsjunkparser.cpp \
    ../core/modules/mp4/mp4sampleentryparser.cpp \
    ../core/modules/mp4/mp4simpleparser.cpp \
    ../core/formatdetector/formatdetector.cpp \
    ../core/interpreter/fromfilemodule.cpp \
    ../core/interpreter/scope.cpp \
    ../core/interpreter/fromfileparser.cpp \
    ../core/interpreter/objectscope.cpp \
    ../core/interpreter/typescope.cpp \
    ../core/interpreter/localscope.cpp \
    ../core/interpreter/compositescope.cpp \
    ../core/modules/default/tupleparser.cpp \
    ../core/interpreter/variabledescriptor.cpp \
    ../core/util/fileutil.cpp \
    ../core/formatdetector/compositeformatdetector.cpp \
    ../core/formatdetector/extensionformatdetector.cpp \
    ../core/formatdetector/magicformatdetector.cpp \
    ../core/formatdetector/syncbyteformatdetector.cpp \
    ../core/formatdetector/standardformatdetector.cpp \
    ../core/interpreter/interpreterconstructor.cpp \
    mt/worker.cpp \
    mt/tasklist.cpp \
    mt/parsingtask.cpp \
    mt/resourcemanager.cpp \
    mt/resource.cpp \
    ../core/modules/default/defaultmodule.cpp \

HEADERS  += mainwindow.h \
    ../core/object.h \
    ../core/file.h \
    ../core/variant.h \
    ../core/objecttypetemplate.h \
    ../core/objecttype.h \
    ../core/module.h \
    ../core/moduleloader.h \
    ../core/modules/mp4/mp4types.h \
    ../core/modules/mp4/mp4module.h \
    ../core/modules/standard/standardtypes.h \
    ../core/modules/standard/standardmodule.h \
    ../core/util/strutil.h \
    ../core/util/csvreader.h \
    tree/htmldelegate.h \
    tree/treewidget.h \
    tree/treeview.h \
    tree/treemodel.h \
    tree/treeobjectitem.h \
    hex/hexfilewidget.h \
    hex/hexfileview.h \
    hex/hexfilemodel.h \
    hex/hexfiledockwidget.h \
    hex/hexfiledelegate.h \
    ../core/util/rapidxml_utils.hpp \
    ../core/util/rapidxml_print.hpp \
    ../core/util/rapidxml_iterators.hpp \
    ../core/util/rapidxml.hpp \
    ../core/modules/mkv/mkvmodule.h \
    ../core/util/bitutil.h \
    tree/treeitem.h \
    ../core/modules/ts/tstypes.h \
    ../core/modules/ts/tsmodule.h \
    hex/hexfileheader.h \
    ../core/modules/ebml/ebmltypes.h \
    ../core/modules/ebml/ebmlmodule.h \
    ../core/modules/hmc/hmcmodule.h \
    ../core/showcase.h \
    ../core/interpreter/interpreter.h \
    tree/filterwidget.h \
    ../core/interpreter/filter.h \
    ../compiler/model.h \
    ../core/interpreter/variable.h \
    ../core/parser.h \
    ../core/modules/default/fileparser.h \
    ../core/modules/standard/bitparser.h \
    ../core/modules/standard/floatparser.h \
    ../core/modules/standard/intparser.h \
    ../core/modules/ts/tsfileparser.h \
    ../core/containerparser.h \
    ../core/modules/ts/tsadaptationfieldparser.h \
    ../core/modules/ts/tspacketparser.h \
    ../core/modules/ebml/ebmlcontainerparser.h \
    ../core/util/iterutil.h \
    ../core/modules/ebml/ebmlsimpleparser.h \
    ../core/modules/ebml/ebmllargeintegerparser.h \
    ../core/modules/ebml/ebmlmasterparser.h \
    ../core/modules/mp4/mp4boxparser.h \
    ../core/mapmodule.h \
    ../core/modules/standard/wordparser.h \
    ../core/modules/default/arrayparser.h \
    ../core/modules/ebml/ebmldateparser.h \
    ../core/interpreter/program.h \
    ../core/modules/ts/tsjunkparser.h \
    ../core/modules/mp4/mp4sampleentryparser.h \
    ../core/modules/mp4/mp4simpleparser.h \
    ../core/formatdetector/formatdetector.h \
    ../core/interpreter/fromfilemodule.h \
    ../core/interpreter/scope.h \
    ../core/interpreter/fromfileparser.h \
    ../core/interpreter/objectscope.h \
    ../core/interpreter/typescope.h \
    ../core/interpreter/localscope.h \
    ../core/interpreter/compositescope.h \
    ../core/modules/default/tupleparser.h \
    ../core/interpreter/variabledescriptor.h \
    ../core/util/fileutil.h \
    ../core/util/unused.h \
    hex/hexfilesearchwidget.h \
    ../core/formatdetector/compositeformatdetector.h \
    ../core/formatdetector/extensionformatdetector.h \
    ../core/formatdetector/magicformatdetector.h \
    ../core/formatdetector/syncbyteformatdetector.h \
    ../core/formatdetector/standardformatdetector.h \
    mt/worker.h \
    mt/tasklist.h \
    mt/parsingtask.h \
    mt/resourcemanager.h \
    mt/resource.h \
    ../core/modules/default/defaultmodule.h \
    ../core/modules/default/defaulttypes.h \
    ../core/interpreter/interpreterconstructor.h

RESOURCES += \
    ressources.qrc

RC_FILE = logo.rc

OTHER_FILES += \
    ../core/modules/mp4model.csv \
    ../core/modules/mp4/mp4model.csv \
    ../core/modules/mkv/mkvmodel.xml \
    ../core/modules/hmc/hmcmodel.csv

DEFINES += USE_QT

QMAKE_CXXFLAGS += -Wno-unused-parameter
