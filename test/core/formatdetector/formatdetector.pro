
QT += testlib widgets
TARGET = test_formatdetector
CONFIG += testcase c++11
TEMPLATE = app
INCLUDEPATH += . \
    ../../../

HEADERS += test_formatdetector.h

SOURCES += \
    test_formatdetector.cpp \
    main.cpp

LIBS += -L../../../core -lhexamonkey
