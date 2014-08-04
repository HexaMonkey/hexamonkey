
! include( ../common_link.pri ) {
error( "Could not find the common_link.pri file!" )
}

CONFIG += c++11 no_include_pwd
CONFIG -= qt

TARGET = hexamonkey-cli
TEMPLATE = app

macx {
  CONFIG -= app_bundle
}

INCLUDEPATH = ..

SOURCES += main.cpp

OTHER_FILES += \
    ../models/mkvmodel.xml \
    ../models/hmcmodel.csv

QMAKE_CXXFLAGS += -Wno-unused-parameter

unix {
    defined(BINDIR, var) {
        target.path = $$prefix.path/$$BINDIR
    } else {
        target.path = $$prefix.path/usr/bin
    }
}
INSTALLS += target
