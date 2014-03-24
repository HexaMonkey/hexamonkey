
TARGET = hexamonkey
TEMPLATE = lib

CONFIG += c++11 no_include_pwd
CONFIG -= qt
win32: CONFIG += static

QMAKE_CXXFLAGS += -Wno-unused-parameter

INCLUDEPATH = ..

SOURCES += \
  variant.cpp \
  showcase.cpp \
  parser.cpp \
  objecttypetemplate.cpp \
  objecttype.cpp \
  object.cpp \
  moduleloader.cpp \
  module.cpp \
  mapmodule.cpp \
  containerparser.cpp \
  error/errormanager.cpp\
  file/fragmentedfile.cpp \
  file/realfile.cpp \
  formatdetector/syncbyteformatdetector.cpp \
  formatdetector/standardformatdetector.cpp \
  formatdetector/magicformatdetector.cpp \
  formatdetector/formatdetector.cpp \
  formatdetector/extensionformatdetector.cpp \
  formatdetector/compositeformatdetector.cpp \
  interpreter/variablepath.cpp \
  interpreter/variable.cpp \
  interpreter/program.cpp \
  interpreter/programloader.cpp \
  interpreter/fromfileparser.cpp \
  interpreter/fromfilemodule.cpp \
  interpreter/filter.cpp \
  interpreter/evaluator.cpp \
  interpreter/blockexecution.cpp \
  interpreter/scope/typescope.cpp \
  interpreter/scope/scope.cpp \
  interpreter/scope/objectscope.cpp \
  interpreter/scope/localscope.cpp \
  interpreter/scope/functionscope.cpp \
  interpreter/scope/compositescope.cpp \
  modules/default/tupleparser.cpp \
  modules/default/fileparser.cpp \
  modules/default/defaultmodule.cpp \
  modules/default/arrayparser.cpp \
  modules/standard/wordparser.cpp \
  modules/standard/standardmodule.cpp \
  modules/standard/intparser.cpp \
  modules/standard/floatparser.cpp \
  modules/standard/bitparser.cpp \
  modules/ebml/ebmlsimpleparser.cpp \
  modules/ebml/ebmlmodule.cpp \
  modules/ebml/ebmlmasterparser.cpp \
  modules/ebml/ebmllargeintegerparser.cpp \
  modules/ebml/ebmldateparser.cpp \
  modules/ebml/ebmlcontainerparser.cpp \
  modules/hmc/hmcmodule.cpp \
  modules/mkv/mkvmodule.cpp \
  util/strutil.cpp \
  util/iterutil.cpp \
  util/fileutil.cpp \
  util/csvreader.cpp \
  util/bitutil.cpp \
  modules/default/dataparser.cpp \
  util/osutil.cpp \
    file/file.cpp

HEADERS  += variant.h \
    showcase.h \
    parser.h \
    objecttypetemplate.h \
    objecttype.h \
    object.h \
    moduleloader.h \
    module.h \
    mapmodule.h \
    containerparser.h \
    error/errormanager.h \
    error/errorobserver.h \
    file/file.h \
    file/fragmentedfile.h \
    file/realfile.h \
    formatdetector/syncbyteformatdetector.h \
    formatdetector/standardformatdetector.h \
    formatdetector/magicformatdetector.h \
    formatdetector/formatdetector.h \
    formatdetector/extensionformatdetector.h \
    formatdetector/compositeformatdetector.h \
    interpreter/variablepath.h \
    interpreter/variable.h \
    interpreter/program.h \
    interpreter/programloader.h \
    interpreter/fromfileparser.h \
    interpreter/fromfilemodule.h \
    interpreter/filter.h \
    interpreter/evaluator.h \
    interpreter/blockexecution.h \
    interpreter/scope/typescope.h \
    interpreter/scope/scope.h \
    interpreter/scope/objectscope.h \
    interpreter/scope/localscope.h \
    interpreter/scope/functionscope.h \
    interpreter/scope/compositescope.h \
    modules/default/tupleparser.h \
    modules/default/fileparser.h \
    modules/default/defaulttypes.h \
    modules/default/defaultmodule.h \
    modules/default/arrayparser.h \
    modules/standard/wordparser.h \
    modules/standard/standardtypes.h \
    modules/standard/standardmodule.h \
    modules/standard/intparser.h \
    modules/standard/floatparser.h \
    modules/standard/bitparser.h \
    modules/ebml/ebmltypes.h \
    modules/ebml/ebmlsimpleparser.h \
    modules/ebml/ebmlmodule.h \
    modules/ebml/ebmlmasterparser.h \
    modules/ebml/ebmllargeintegerparser.h \
    modules/ebml/ebmldateparser.h \
    modules/ebml/ebmlcontainerparser.h \
    modules/hmc/hmcmodule.h \
    modules/mkv/mkvtypes.h \
    modules/mkv/mkvmodule.h \
    util/unused.h \
    util/strutil.h \
    util/iterutil.h \
    util/fileutil.h \
    util/csvreader.h \
    util/bitutil.h \
    util/rapidxml/rapidxml_utils.hpp \
    util/rapidxml/rapidxml_print.hpp \
    util/rapidxml/rapidxml_iterators.hpp \
    util/rapidxml/rapidxml.hpp \
    ../compiler/model.h \
    modules/default/dataparser.h \
    util/ptrutil.h \
    util/osutil.h

unix {
    defined(LIBDIR, var) {
        target.path = $$prefix.path/$$LIBDIR
    } else {
        target.path = $$prefix.path/usr/lib
    }
}
INSTALLS += target
