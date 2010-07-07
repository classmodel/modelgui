TARGET = mlmodel_main
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    model.cpp \
    modelrun.cpp \
    plotwindow.cpp \
    subplot.cpp \
    modeloutput.cpp \
    switches.cpp \
    defaultinput.cpp \
    landsoil.cpp
HEADERS += mainwindow.h \
    modeloutput.h \
    modelinput.h \
    model.h \
    modelrun.h \
    plotwindow.h \
    subplot.h \
    landsoil.h
FORMS += mainwindow.ui \
    plotwindow.ui \
    subplot.ui
