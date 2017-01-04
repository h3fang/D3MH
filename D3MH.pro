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

LIBS += -lwinmm

SOURCES += main.cpp\
    minimap.cpp \
    modules/engine.cpp \
    modules/navmesh.cpp \
    modules/snocache.cpp \
    modules/helpers.cpp \
    modules/autoloot.cpp \
    utils/memoryreader.cpp \
    utils/helper.cpp \
    utils/hook.cpp \
    utils/keyboardmouse.cpp

HEADERS  += minimap.h \
    datatypes/objectmanager.h \
    datatypes/structs.h \
    datatypes/types.h \
    datatypes/localdata.h \
    datatypes/scene.h \
    datatypes/actor.h \
    datatypes/sno.h \
    datatypes/levelarea.h \
    datatypes/marker.h \
    datatypes/ui.h \
    datatypes/actorcommondata.h \
    modules/engine.h \
    modules/navmesh.h \
    modules/snocache.h \
    modules/helpers.h \
    modules/autoloot.h \
    utils/memoryreader.h \
    utils/pointer.h \
    utils/hook.h \
    utils/keyboardmouse.h \
    utils/point.h \
    datatypes/enums.h \
    datatypes/attributedescriptor.h \
    utils/helpers.h

RC_FILE = default.rc

QMAKE_CXXFLAGS_RELEASE += -msse2 -mfpmath=sse #-DQT_NO_DEBUG_OUTPUT
