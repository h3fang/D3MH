#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T12:02:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HPMsg64
TEMPLATE = app

DESTDIR = ../bin

CONFIG += c++14

INCLUDEPATH += ../ISS/include

SOURCES += main.cpp\
    minimap.cpp \
    process/memoryreader.cpp \
    process/helper.cpp \
    modules/engine.cpp \
    modules/navmesh.cpp \
    modules/snocache.cpp \
    modules/helpers.cpp

HEADERS  += minimap.h \
    datatypes/objectmanager.h \
    datatypes/structs.h \
    datatypes/types.h \
    datatypes/localdata.h \
    datatypes/scene.h \
    datatypes/actor.h \
    datatypes/sno.h \
    datatypes/snoenum.h \
    datatypes/levelarea.h \
    datatypes/marker.h \
    datatypes/ui.h \
    datatypes/actorcommondata.h \
    process/helper.h \
    process/memoryreader.h \
    process/pointer.h \
    modules/engine.h \
    modules/navmesh.h \
    modules/snocache.h \
    modules/helpers.h

RC_FILE = default.rc

QMAKE_CXXFLAGS_RELEASE += -msse2 -mfpmath=sse #-DQT_NO_DEBUG_OUTPUT
