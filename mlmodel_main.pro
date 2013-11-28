#
# CLASS
# Copyright (c) 2010-2013 Meteorology and Air Quality section, Wageningen University and Research centre
# Copyright (c) 2011-2013 Jordi Vila-Guerau de Arellano
# Copyright (c) 2011-2013 Chiel van Heerwaarden
# Copyright (c) 2011-2013 Bart van Stratum
# Copyright (c) 2011-2013 Kees van den Dries
#
# This file is part of CLASS
#
# CLASS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CLASS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CLASS.  If not, see <http://www.gnu.org/licenses/>.
#

TARGET = CLASS
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
    landsoil.cpp \
    modelchem.cpp \
    modelinput.cpp
HEADERS += mainwindow.h \
    modeloutput.h \
    modelinput.h \
    model.h \
    modelrun.h \
    plotwindow.h \
    subplot.h \
    landsoil.h \
    modelchemtypes.h \
    modelchem.h
FORMS += mainwindow.ui \
    plotwindow.ui \
    subplot.ui

greaterThan(QT_MAJOR_VERSION, 4)
{
  QT += widgets
  QT += printsupport
}
