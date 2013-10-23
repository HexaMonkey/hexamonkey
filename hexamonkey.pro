
TEMPLATE = subdirs


CONFIG += ordered

SUBDIRS = compiler \
          core \
          cli \
          gui

compiler.commands = make -C compiler
QMAKE_EXTRA_TARGETS += compiler
compiler.CONFIG = phony
