# tm_profile - program for calculation of cylinder-bevel transmissions
# Copyright (C) 2023 Sergei Lopatin
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

QT       += qml quick widgets

TARGET = tm_profile
TEMPLATE = app

win32:VERSION = 1.0.0

SOURCES += main.cpp\
    profile.cpp \
    imagegenerator.cpp

HEADERS  += \
    profile.h \
    imagegenerator.h

TRANSLATIONS += tm_profile_ru.ts

RESOURCES += \
    qml.qrc

