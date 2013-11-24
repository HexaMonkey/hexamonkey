
! include( ../../test_common.pri ) {
    error( Could not find the test_common.pri file! )
}

INCLUDEPATH += ../../../

HEADERS += test_util.h

SOURCES += test_util.cpp

LIBS += -L../../../core -lhexamonkey
