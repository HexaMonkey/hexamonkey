
TARGET = hexamonkey
TEMPLATE = lib

CONFIG += c++11 no_include_pwd
CONFIG -= qt
win32: CONFIG += static
macx: CONFIG += static

QMAKE_CXXFLAGS += -Wno-unused-parameter

! include(core.pri) {
	error( "Could not find the core.pri file!" )
}

unix {
    defined(LIBDIR, var) {
        target.path = $$prefix.path/$$LIBDIR
    } else {
        target.path = $$prefix.path/usr/lib
    }
}
INSTALLS += target
