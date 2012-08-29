#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T20:46:46
#
#-------------------------------------------------

QT       -= core gui

Debug:TARGET = ../../bin/libviu2x_d
Release:TARGET = ../../bin/libviu2x
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    viuIO/FileSystem.cpp \
    viuIO/Path.cpp

HEADERS += \
    viuIO/Path.h \
    viuIO/FileSystem.h

INCLUDEPATH += viufs

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
