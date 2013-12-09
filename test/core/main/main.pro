
! include( ../../test_common.pri ) {
    error( Could not find the test_common.pri file! )
}

! include( ../../../common_link.pri ) {
    error( Could not find the common_link.pri file! )
}

INCLUDEPATH += ../../../

HEADERS += test_variant.h

SOURCES += main.cpp \
    test_variant.cpp

