
QT += testlib widgets
TARGET = test_util
CONFIG += testcase c++11
TEMPLATE = app
INCLUDEPATH += . \
    ../../../

HEADERS += test_util.h

SOURCES += test_util.cpp

LIBS += -L../../../core -lhexamonkey
