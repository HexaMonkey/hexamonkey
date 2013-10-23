
CONFIG += c++11
CONFIG -= qt

TARGET = hexamonkey-cli
TEMPLATE = app

INCLUDEPATH = ..

SOURCES += main.cpp

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
    ../core/util/rapidxml_utils.hpp \
    ../core/util/rapidxml_print.hpp \
    ../core/util/rapidxml_iterators.hpp \
    ../core/util/rapidxml.hpp \
    ../compiler/model.h \
    ../core/modules/default/dataparser.h \
    ../core/util/ptrutil.h \
    ../core/util/osutil.h

OTHER_FILES += \
    ../models/mkvmodel.xml \
    ../models/hmcmodel.csv

QMAKE_CXXFLAGS += -Wno-unused-parameter

LIBS += -L../core -lhexamonkey
