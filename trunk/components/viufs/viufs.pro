#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T20:46:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = viufs
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    source/ViuFS.cpp

HEADERS += \
    include/ViuFS.h \
    include/ViuPath.h

INCLUDEPATH += include

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
