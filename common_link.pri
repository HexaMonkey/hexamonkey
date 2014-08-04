
SRCDIR = $$PWD

unix: LIBS += -L$$SRCDIR/core -lhexamonkey
win32 {
    CONFIG( debug, debug|release ) {
        LIBS += -L$$SRCDIR/core/debug -lhexamonkey
    } else {
        LIBS += -L$$SRCDIR/core/release -lhexamonkey
    }
}


