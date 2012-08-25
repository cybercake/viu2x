#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T20:46:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = libviu2x
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    viufs/source/FileSystem.cpp \
    viufs/source/Path.cpp

HEADERS += \
    viufs/include/FileSystem.h \
    viufs/include/Path.h

INCLUDEPATH += viufs/include

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
