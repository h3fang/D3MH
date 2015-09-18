#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T12:02:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = D3MH
TEMPLATE = app

DESTDIR = ../bin

CONFIG += c++11

SOURCES += main.cpp\
        minimap.cpp \
    engine/engine.cpp \
    engine/memoryreader.cpp \
    engine/helper.cpp

HEADERS  += minimap.h \
    engine/engine.h \
    engine/objectmanager.h \
    engine/structs.h \
    engine/types.h \
    engine/localdata.h \
    engine/scene.h \
    engine/memoryreader.h \
    engine/helper.h \
    engine/actor.h

FORMS    += minimap.ui

RC_FILE = default.rc
