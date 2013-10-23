
TEMPLATE = subdirs


CONFIG += ordered

SUBDIRS = compiler \
          core \
          gui

compiler.commands = make -C compiler
QMAKE_EXTRA_TARGETS += compiler
compiler.CONFIG = phony
