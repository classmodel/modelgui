# -------------------------------------------------
# Project created by QtCreator 2009-11-28T15:55:09
# Bart van Stratum, nov 2009.
# -------------------------------------------------
TARGET = mxl2
TEMPLATE = app
SOURCES += main.cpp \
    widget.cpp
HEADERS += widget.h
FORMS += widget.ui
OTHER_FILES += vars.ini

# install
target.path = /home/bart/QT/projectsBvS/mxl2
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.ui *.pro
sources.path = /home/bart/QT/projectsBvS/mxl2
INSTALLS += target sources
