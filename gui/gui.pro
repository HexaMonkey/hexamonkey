#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T17:52:05
#
#-------------------------------------------------

! include( ../common_link.pri ) {
error( "Could not find the common_link.pri file!" )
}

QT       += core gui widgets

CONFIG   += c++11 no_include_pwd

TARGET = hexamonkey
TEMPLATE = app

INCLUDEPATH = ..

SOURCES += main.cpp\
        mainwindow.cpp \
    tree/treewidget.cpp \
    tree/treeview.cpp \
    tree/treeobjectitem.cpp \
    tree/treemodel.cpp \
    tree/treeitem.cpp \
    tree/htmldelegate.cpp \
    tree/filterwidget.cpp \
    hex/hexfilewidget.cpp \
    hex/hexfileview.cpp \
    hex/hexfilesearchwidget.cpp \
    hex/hexfilemodel.cpp \
    hex/hexfileheader.cpp \
    hex/hexfiledelegate.cpp \
    mt/worker.cpp \
    mt/tasklist.cpp \
    mt/resourcemanager.cpp \
    mt/resource.cpp \
    mt/parsingtask.cpp \
    tree/treefileitem.cpp \
    qtprogramloader.cpp


		
HEADERS  += mainwindow.h \
    tree/treewidget.h \
    tree/treeview.h \
    tree/treeobjectitem.h \
    tree/treemodel.h \
    tree/treeitem.h \
    tree/htmldelegate.h \
    tree/filterwidget.h \
    hex/hexfilewidget.h \
    hex/hexfileview.h \
    hex/hexfilesearchwidget.h \
    hex/hexfilemodel.h \
    hex/hexfileheader.h \
    hex/hexfiledelegate.h \
    mt/worker.h \
    mt/tasklist.h \
    mt/resourcemanager.h \
    mt/resource.h \
    mt/parsingtask.h \
    tree/treefileitem.h \
    qtprogramloader.h

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
