#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T12:02:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demh
TEMPLATE = app

DESTDIR = ../bin

CONFIG += c++14

INCLUDEPATH += ../ISS/include

SOURCES += main.cpp\
    minimap.cpp \
    engine/engine.cpp \
    process/memoryreader.cpp \
    process/helper.cpp \
    engine/navmesh.cpp \
    module/snocache.cpp

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
    process/pointer.h \
    engine/levelarea.h \
    engine/marker.h \
    engine/ui.h \
    engine/actorcommondata.h \
    module/snocache.h

RC_FILE = default.rc

QMAKE_CXXFLAGS_RELEASE += -msse2 -mfpmath=sse #-DQT_NO_DEBUG_OUTPUT
