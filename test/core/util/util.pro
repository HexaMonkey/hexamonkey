
! include( ../../test_common.pri ) {
    error( Could not find the test_common.pri file! )
}

! include( ../../../common_link.pri ) {
    error( Could not find the common_link.pri file! )
}

INCLUDEPATH += ../../../

HEADERS += test_util.h

SOURCES += test_util.cpp
