#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T12:02:02
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = D3MH
TEMPLATE = app

DESTDIR = ../bin

CONFIG += c++11

SOURCES += main.cpp\
        minimap.cpp \
    engine/engine.cpp \
    process/memoryreader.cpp \
    process/helper.cpp \
    engine/navmesh.cpp

HEADERS  += minimap.h \
    engine/engine.h \
    engine/objectmanager.h \
    engine/structs.h \
    engine/types.h \
    engine/localdata.h \
    engine/scene.h \
    engine/actor.h \
    engine/sno.h \
    engine/snoenum.h \
    process/helper.h \
    process/memoryreader.h \
    engine/navmesh.h \
    process/pointer.h

FORMS    +=

LIBS += -lopengl32

RC_FILE = default.rc
