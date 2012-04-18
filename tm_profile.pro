# tm_profile - program for calculation of cylinder-bevel transmissions
# Copyright (C) 2011 Sergey Lopatin
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Add more folders to ship with the application, here
folder_01.source = qml/tm_profile
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QML_IMPORT_PATH =

QT       += core gui

TARGET = tm_profile
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    profile.cpp \
    paint_area.cpp \
    mythread.cpp

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS  += mainwindow.h \
    profile.h \
    paint_area.h \
    mythread.h

FORMS    += mainwindow.ui
