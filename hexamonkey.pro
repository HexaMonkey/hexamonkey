
TEMPLATE = subdirs


CONFIG += ordered

unix: SUBDIRS += compiler
SUBDIRS += core \
           gui

scripts.files = scripts/*.hm
unix: scripts.path = $$prefix.path/usr/share/hexamonkey/scripts
INSTALLS += scripts

resources.files = \
  models/hmcmodel.csv \
  models/mkvmodel.xml \
  gui/logo.svg
unix: resources.path = $$prefix.path/usr/share/hexamonkey
INSTALLS += resources
