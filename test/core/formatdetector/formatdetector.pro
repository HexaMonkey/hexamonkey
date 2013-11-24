
! include( ../../test_common.pri ) {
    error( Could not find the test_common.pri file! )
}

INCLUDEPATH += ../../../

HEADERS += test_formatdetector.h

SOURCES += test_formatdetector.cpp

LIBS += -L../../../core -lhexamonkey
