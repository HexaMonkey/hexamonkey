#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T17:52:05
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG   += c++11

TARGET = hexamonkey
TEMPLATE = app

INCLUDEPATH += ../core \
               ../core/modules/default \
               ../core/modules/standard \
               ../core/modules/ebml \
               ../core/modules/mkv \
               ../core/modules/hmc \
               ../core/formatdetector \
               ../core/util \
               ../core/interpreter \
               ../core/interpreter/scope \
               ../compiler \
               tree \
               hex \
               mt

SOURCES += main.cpp\
        mainwindow.cpp \
    ../core/variant.cpp \
    ../core/showcase.cpp \
    ../core/parser.cpp \
    ../core/objecttypetemplate.cpp \
    ../core/objecttype.cpp \
    ../core/object.cpp \
    ../core/moduleloader.cpp \
    ../core/module.cpp \
    ../core/mapmodule.cpp \
    ../core/file.cpp \
    ../core/containerparser.cpp \
    ../core/formatdetector/syncbyteformatdetector.cpp \
    ../core/formatdetector/standardformatdetector.cpp \
    ../core/formatdetector/magicformatdetector.cpp \
    ../core/formatdetector/formatdetector.cpp \
    ../core/formatdetector/extensionformatdetector.cpp \
    ../core/formatdetector/compositeformatdetector.cpp \
    ../core/interpreter/variabledescriptor.cpp \
    ../core/interpreter/variable.cpp \
    ../core/interpreter/program.cpp \
    ../core/interpreter/interpreterconstructor.cpp \
    ../core/interpreter/interpreter.cpp \
    ../core/interpreter/holder.cpp \
    ../core/interpreter/fromfileparser.cpp \
    ../core/interpreter/fromfilemodule.cpp \
    ../core/interpreter/filter.cpp \
    ../core/interpreter/blockexecution.cpp \
    ../core/interpreter/scope/typescope.cpp \
    ../core/interpreter/scope/scope.cpp \
    ../core/interpreter/scope/objectscope.cpp \
    ../core/interpreter/scope/localscope.cpp \
    ../core/interpreter/scope/functionscope.cpp \
    ../core/interpreter/scope/compositescope.cpp \
    ../core/modules/default/tupleparser.cpp \
    ../core/modules/default/fileparser.cpp \
    ../core/modules/default/defaultmodule.cpp \
    ../core/modules/default/arrayparser.cpp \
    ../core/modules/standard/wordparser.cpp \
    ../core/modules/standard/standardmodule.cpp \
    ../core/modules/standard/intparser.cpp \
    ../core/modules/standard/floatparser.cpp \
    ../core/modules/standard/bitparser.cpp \
    ../core/modules/ebml/ebmlsimpleparser.cpp \
    ../core/modules/ebml/ebmlmodule.cpp \
    ../core/modules/ebml/ebmlmasterparser.cpp \
    ../core/modules/ebml/ebmllargeintegerparser.cpp \
    ../core/modules/ebml/ebmldateparser.cpp \
    ../core/modules/ebml/ebmlcontainerparser.cpp \
    ../core/modules/hmc/hmcmodule.cpp \
    ../core/modules/mkv/mkvmodule.cpp \
    ../core/util/strutil.cpp \
    ../core/util/iterutil.cpp \
    ../core/util/fileutil.cpp \
    ../core/util/csvreader.cpp \
    ../core/util/bitutil.cpp \
    tree/treewidget.cpp \
    tree/treeview.cpp \
    tree/treeobjectitem.cpp \
    tree/treemodel.cpp \
    tree/treeitem.cpp \
    tree/htmldelegate.cpp \
    tree/filterwidget.cpp \
    hex/hexfilewidget.cpp \
    hex/hexfileview.cpp \
    hex/hexfilesearchwidget.cpp \
    hex/hexfilemodel.cpp \
    hex/hexfileheader.cpp \
    hex/hexfiledockwidget.cpp \
    hex/hexfiledelegate.cpp \
    mt/worker.cpp \
    mt/tasklist.cpp \
    mt/resourcemanager.cpp \
    mt/resource.cpp \
    mt/parsingtask.cpp \
    ../core/modules/default/dataparser.cpp


		
HEADERS  += mainwindow.h \ 
    ../core/variant.h \
    ../core/showcase.h \
    ../core/parser.h \
    ../core/objecttypetemplate.h \
    ../core/objecttype.h \
    ../core/object.h \
    ../core/moduleloader.h \
    ../core/module.h \
    ../core/mapmodule.h \
    ../core/file.h \
    ../core/containerparser.h \
    ../core/formatdetector/syncbyteformatdetector.h \
    ../core/formatdetector/standardformatdetector.h \
    ../core/formatdetector/magicformatdetector.h \
    ../core/formatdetector/formatdetector.h \
    ../core/formatdetector/extensionformatdetector.h \
    ../core/formatdetector/compositeformatdetector.h \
    ../core/interpreter/variabledescriptor.h \
    ../core/interpreter/variable.h \
    ../core/interpreter/program.h \
    ../core/interpreter/interpreterconstructor.h \
    ../core/interpreter/interpreter.h \
    ../core/interpreter/fromfileparser.h \
    ../core/interpreter/fromfilemodule.h \
    ../core/interpreter/filter.h \
    ../core/interpreter/blockexecution.h \
    ../core/interpreter/scope/typescope.h \
    ../core/interpreter/scope/scope.h \
    ../core/interpreter/scope/objectscope.h \
    ../core/interpreter/scope/localscope.h \
    ../core/interpreter/holder.h \
    ../core/interpreter/scope/functionscope.h \
    ../core/interpreter/scope/compositescope.h \
    ../core/modules/default/tupleparser.h \
    ../core/modules/default/fileparser.h \
    ../core/modules/default/defaulttypes.h \
    ../core/modules/default/defaultmodule.h \
    ../core/modules/default/arrayparser.h \
    ../core/modules/standard/wordparser.h \
    ../core/modules/standard/standardtypes.h \
    ../core/modules/standard/standardmodule.h \
    ../core/modules/standard/intparser.h \
    ../core/modules/standard/floatparser.h \
    ../core/modules/standard/bitparser.h \
    ../core/modules/ebml/ebmltypes.h \
    ../core/modules/ebml/ebmlsimpleparser.h \
    ../core/modules/ebml/ebmlmodule.h \
    ../core/modules/ebml/ebmlmasterparser.h \
    ../core/modules/ebml/ebmllargeintegerparser.h \
    ../core/modules/ebml/ebmldateparser.h \
    ../core/modules/ebml/ebmlcontainerparser.h \
    ../core/modules/hmc/hmcmodule.h \
    ../core/modules/mkv/mkvtypes.h \
    ../core/modules/mkv/mkvmodule.h \
    ../core/util/unused.h \
    ../core/util/strutil.h \
    ../core/util/str.h \
    ../core/util/iterutil.h \
    ../core/util/fileutil.h \
    ../core/util/csvreader.h \
    ../core/util/bitutil.h \
    ../core/util/rapidxml_utils.hpp \
    ../core/util/rapidxml_print.hpp \
    ../core/util/rapidxml_iterators.hpp \
    ../core/util/rapidxml.hpp \
    tree/treewidget.h \
    tree/treeview.h \
    tree/treeobjectitem.h \
    tree/treemodel.h \
    tree/treeitem.h \
    tree/htmldelegate.h \
    tree/filterwidget.h \
    hex/hexfilewidget.h \
    hex/hexfileview.h \
    hex/hexfilesearchwidget.h \
    hex/hexfilemodel.h \
    hex/hexfileheader.h \
    hex/hexfiledockwidget.h \
    hex/hexfiledelegate.h \
    mt/worker.h \
    mt/tasklist.h \
    mt/resourcemanager.h \
    mt/resource.h \
    mt/parsingtask.h \
    ../compiler/model.h \
    ../core/modules/default/dataparser.h

RESOURCES += \
    ressources.qrc

RC_FILE = logo.rc

OTHER_FILES += \
    ../core/modules/mkv/mkvmodel.xml \
    ../core/modules/hmc/hmcmodel.csv

DEFINES += USE_QT

QMAKE_CXXFLAGS += -Wno-unused-parameter
