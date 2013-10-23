
TARGET = hexamonkey
TEMPLATE = lib

CONFIG += c++11
CONFIG -= qt

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
  file.cpp \
  containerparser.cpp \
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
  util/osutil.cpp
