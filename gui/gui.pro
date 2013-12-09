#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T17:52:05
#
#-------------------------------------------------

! include( ../common_link.pri ) {
error( "Could not find the common_link.pri file!" )
}

QT       += core gui widgets

CONFIG   += c++11 no_include_pwd

TARGET = hexamonkey
TEMPLATE = app

INCLUDEPATH = ..

SOURCES += main.cpp\
        mainwindow.cpp \
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
    hex/hexfiledelegate.cpp \
    mt/worker.cpp \
    mt/tasklist.cpp \
    mt/resourcemanager.cpp \
    mt/resource.cpp \
    mt/parsingtask.cpp \
    tree/treefileitem.cpp \


		
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
    ../core/interpreter/variablepath.h \
    ../core/interpreter/variable.h \
    ../core/interpreter/program.h \
    ../core/interpreter/programloader.h \
    ../core/interpreter/fromfileparser.h \
    ../core/interpreter/fromfilemodule.h \
    ../core/interpreter/filter.h \
    ../core/interpreter/evaluator.h \
    ../core/interpreter/blockexecution.h \
    ../core/interpreter/scope/typescope.h \
    ../core/interpreter/scope/scope.h \
    ../core/interpreter/scope/objectscope.h \
    ../core/interpreter/scope/localscope.h \
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
    ../core/util/iterutil.h \
    ../core/util/fileutil.h \
    ../core/util/csvreader.h \
    ../core/util/bitutil.h \
    ../core/util/rapidxml/rapidxml_utils.hpp \
    ../core/util/rapidxml/rapidxml_print.hpp \
    ../core/util/rapidxml/rapidxml_iterators.hpp \
    ../core/util/rapidxml/rapidxml.hpp \
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
    hex/hexfiledelegate.h \
    mt/worker.h \
    mt/tasklist.h \
    mt/resourcemanager.h \
    mt/resource.h \
    mt/parsingtask.h \
    ../compiler/model.h \
    ../core/modules/default/dataparser.h \
    ../core/util/ptrutil.h \
    tree/treefileitem.h \
    ../core/util/osutil.h

RESOURCES += \
    ressources.qrc

RC_FILE = logo.rc

OTHER_FILES += \
    ../models/mkvmodel.xml \
    ../models/hmcmodel.csv

DEFINES += USE_QT

QMAKE_CXXFLAGS += -Wno-unused-parameter

unix {
    defined(BINDIR, var) {
        target.path = $$prefix.path/$$BINDIR
    } else {
        target.path = $$prefix.path/usr/bin
    }
}
INSTALLS += target
