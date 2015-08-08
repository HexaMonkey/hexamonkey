unix {
	! include( ../common_link.pri ) {
		error( "Could not find the common_link.pri file!" )
	}
	DEPENDPATH += ..
}

win32 {
	! include(../core/core.pri) {
		error( "Could not find the core.pri file!" )
	}
}

QT += testlib widgets
TARGET = run_test
CONFIG += testcase c++11
TEMPLATE = app
target.CONFIG += no_default_install

HEADERS += \
	test_variant.h \
	test_formatdetector.h \
	test_util.h \
        test_parser.h \
        ../gui/qtmodulesetup.h \
        ../gui/qtprogramloader.h \
    test_variable.h

SOURCES += \
	main.cpp \
        test_variant.cpp \
	test_formatdetector.cpp \
	test_util.cpp \
        test_parser.cpp \
        ../gui/qtmodulesetup.cpp \
        ../gui/qtprogramloader.cpp \
    test_variable.cpp
