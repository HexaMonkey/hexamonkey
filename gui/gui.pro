#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T17:52:05
#
#-------------------------------------------------

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

QT       += core gui widgets

CONFIG   += c++11 no_include_pwd

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

TARGET = hexamonkey
TEMPLATE = app

INCLUDEPATH += ..

SOURCES += \ 
	main.cpp\
    mainwindow.cpp \
    tree/treewidget.cpp \
    tree/treeview.cpp \
    tree/treeobjectitem.cpp \
    tree/treemodel.cpp \
    tree/treefileitem.cpp \
    tree/treeitem.cpp \
    tree/htmldelegate.cpp \
    tree/filterwidget.cpp \
    hex/hexfilewidget.cpp \
    hex/hexfileview.cpp \
    hex/hexfilesearchwidget.cpp \
    hex/hexfilemodel.cpp \
    hex/hexfileheader.cpp \
    hex/hexfiledelegate.cpp \
    log/logwidget.cpp \
    qtprogramloader.cpp \
    thread/threadqueue.cpp \
    thread/functionthread.cpp \
    qtmodulesetup.cpp


HEADERS  += \ 
    mainwindow.h \
    tree/treewidget.h \
    tree/treeview.h \
    tree/treeobjectitem.h \
    tree/treemodel.h \
    tree/treefileitem.h \
    tree/treeitem.h \
    tree/htmldelegate.h \
    tree/filterwidget.h \
    hex/hexfilewidget.h \
    hex/hexfileview.h \
    hex/hexfilesearchwidget.h \
    hex/hexfilemodel.h \
    hex/hexfileheader.h \
    hex/hexfiledelegate.h \
    log/logwidget.h \
    qtprogramloader.h \
    thread/threadqueue.h \
    thread/functionthread.h \
    qtmodulesetup.h

RESOURCES += \
    ressources.qrc

RC_FILE = logo.rc

DEFINES += USE_QT

QMAKE_CXXFLAGS += -Wno-unused-parameter

unix {
    defined(BINDIR, var) {
        target.path = $$prefix.path/$$BINDIR
    } else {
        target.path = $$prefix.path/usr/bin
    }
}
INSTALLS += target
