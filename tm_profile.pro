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

include(qtquick2applicationviewer.pri)
qtcAddDeployment()

QT       += widgets qml quick

TARGET = tm_profile
TEMPLATE = app


SOURCES += main.cpp\
    profile.cpp \
    imagegenerator.cpp

HEADERS  += \
    profile.h \
    imagegenerator.h

TRANSLATIONS += tm_profile_ru.ts

OTHER_FILES += \
    qml/tm_profile/main.qml \
    qml/tm_profile/Fields.qml \
    qml/tm_profile/Buttons.qml \
    qml/tm_profile/Alert.qml \
    qml/tm_profile/Parameters.qml \
    qml/tm_profile/XtList.qml \
    qtquick2applicationviewer.pri

