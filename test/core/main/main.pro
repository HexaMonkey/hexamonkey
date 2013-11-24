
! include( ../../test_common.pri ) {
    error( Could not find the test_common.pri file! )
}

INCLUDEPATH += ../../../

HEADERS += test_variant.h

SOURCES += main.cpp \
    test_variant.cpp

LIBS += -L../../../core -lhexamonkey
